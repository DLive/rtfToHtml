// DLrtfhtml.cpp : 实现文件
//

#include "stdafx.h"
#include "rtftest2.h"
#include "DLrtfhtml.h"


// DLrtfhtml

DLrtfhtml::DLrtfhtml()
{
}

DLrtfhtml::~DLrtfhtml()
{
}
void DLrtfhtml::openfile(CString filename)
{
	CStdioFile rtf;
	rtf.Open(_T("F:\\\\itbook2.tit"),CStdioFile::modeRead);
	int len=rtf.GetLength();
	rtf.SeekToBegin();
	CStringA content;
	rtf.Read(content.GetBuffer(len),len);
	content.ReleaseBuffer();
	rtf.Close();

	char* str,*strd;
	str=strd=new char[len+1];
	memset(str,'\0',sizeof(str));
	int bg,end;
	bg=end=0;
	char* p;
	while((end=content.Find("\\\'",end))>=0)
	{
		if(end==0 || content.GetAt(end-1)!='\\')
		{//转汉字
			CStringA s;
			if(end!=bg)
			{
				strcpy(str,(LPSTR)(LPCSTR)content.Mid(bg,end-bg));
				str+=(end-bg);
			}
			*str=strtol(content.Mid(end+2,2),&p,16);
			str++;
			bg=end+4;
		}
		else
		{// \\' 去斜杆
			if(end!=bg)
			{
				strcpy(str,(LPSTR)(LPCSTR)content.Mid(bg,end-bg-1)); //   '之前还有两个" \ "
				str+=(end-bg-1);
			}
			strcpy(str,(LPSTR)(LPCSTR)content.Mid(end,2));
			str+=2;
			bg=end+2;
		}
		end++;
	}
	int leng=content.GetLength();
	if((content.GetLength()-bg)>2)//如果再最后两个字符找到\'则会＝2  当然根据rtf文档绝对不会这样。
		strcpy(str,(LPSTR)(LPCSTR)content.Mid(bg));
	//}
	int  unicodeLen = ::MultiByteToWideChar( CP_ACP,0,strd,-1,NULL,0);  
	MultiByteToWideChar(CP_ACP,0,strd,-1,(LPWSTR)destcon.GetBuffer(unicodeLen),unicodeLen);
	destcon.ReleaseBuffer();
}
CString DLrtfhtml::beginChange(CString rtfPath)
{
	openfile(rtfPath);
	readfontgroup();
	readcolor();
	getDefaultProperty();
	return toHtmlTag();
}
void DLrtfhtml::readfontgroup()
{
	CString font;
	int start=destcon.Find(_T("\\fonttbl"));
	int pend;//一组字体的结束
	BOOL closeUp=FALSE;//大括号是否闭合
	fontgroup ftemp;
	int id=0;

	int p1,p2,q;
	CString name;
	wchar_t* p;
	CString temp;
	while((start=destcon.Find(_T("{"),start)) && !closeUp)
	{
		pend=destcon.Find(_T("}"),start);
		//CString aa=destcon.GetAt(pend+2);
		if(destcon.GetAt(pend+1)=='}')
			closeUp=TRUE;
		//读取字体间距
		temp=destcon.Mid(start,pend-start);
		ftemp.fprq=_tcstol(temp.Mid( temp.Find(_T("\\fprq"))+5,3 ),&p,10);
		
		//读取字体名称
		CString fname;
		p1=temp.Find(_T(" "));
		p2=temp.Find(_T(";"));
		name=temp.Mid(p1+1,p2-p1-1);
		memset(ftemp.fname,'\0',sizeof(ftemp.fname));
		lstrcpy(ftemp.fname,name.GetBuffer());
		name.ReleaseBuffer();
		ftemp.fid=id;
		m_fg.SetAtGrow(id,ftemp);

		start++;
		id++;
	}

}
BOOL DLrtfhtml::readcolor()
{
	int start=destcon.Find(_T("\\colortbl"));
	if(start<0)
		return FALSE;
	int end=destcon.Find(_T("}"),start);
	CString colorstr=destcon.Mid(start,end-start-1);
	COLORREF color;
	wchar_t* p;
	BYTE r,g,b;
	start=0;
	int i=0;
	int len=colorstr.GetLength();
	while((start=colorstr.Find(_T(";"),start))>=0)
	{
		r=_tcstol(colorstr.Mid(colorstr.Find(_T("red"),start)+3,3),&p,10);
		g=_tcstol(colorstr.Mid(colorstr.Find(_T("green"),start)+5,3),&p,10);
		b=_tcstol(colorstr.Mid(colorstr.Find(_T("blue"),start)+4),&p,10);
		color=RGB(r,g,b);
		m_cg.SetAtGrow(i,color);
		start++;
		i++;
	}
	return TRUE;
}
//
CString DLrtfhtml::toHtmlTag()
{
	CString tstr;
	CString desthtml;
	int pstart=destcon.Find(_T("\\viewkind"));
	pstart=destcon.Find(_T(" "),pstart)+1;
	
	int pend;
	int proB,proE;//中间属性位置

	CString temp,paragraph;//段落
	CStringArray proArr;
	while((pend=destcon.Find(_T("\\par"),pstart))>=0)//获取每一个段落
	{

		//pend=destcon.Find(_T("\\par"),pstart);
		if(pstart>=pend)
			break;
		tstr=destcon.Mid(pstart,pend-pstart);
		paragraph.Format(_T("<p>"));
		proB=proE=0;
		while((proB=getProPosition(tstr,proB))!=-1)
		{
			if(proB!=proE && proB>0)
			{
				temp=tstr.Mid(proE,proB-proE); //proE 上次结束的位置
				if(!temp.IsEmpty())
				{
					addHtmlTagToStr(temp,m_curPro);
					paragraph+=temp;
				}
				//desthtml.Append(tstr.Mid(proB,proE-proB));

			}
			//添加属性到数组里
			proE=tstr.Find(_T(" "),proB);
			if(proE==-1)
				proE=tstr.GetLength();
			stringSplit(tstr.Mid(proB,proE-proB),proArr,_T("\\"));
			addProToArr(proArr,m_curPro);
			proB=proE+1;  //加1 是为了去除属性后的一个空格
		}
		if((tstr.GetLength()-1)>proE)
		{
			temp=tstr.Mid(proE+1);
			addHtmlTagToStr(temp,m_curPro);
			paragraph+=temp;
		}
		paragraph+=_T("</p>");
		//paragraph.Format(_T("%s%s</p>"),paragraph,temp);
		desthtml.Append(paragraph);
		pstart=pend+4;
	}
	return desthtml;
}

//得到默认属性
void DLrtfhtml::getDefaultProperty()
{
	 CString tstr;
	 int pstart=destcon.Find(_T("\\viewkind"));
	 tstr=destcon.Mid(pstart,destcon.Find(_T(" "),pstart)-pstart);
	 CStringArray proArr;
	 stringSplit(tstr,proArr,_T("\\"));
	 addProToArr(proArr,m_defaultPro);//添加默认属性
}
void DLrtfhtml::addProToArr(CStringArray &arr,CArray<rtfProperty,rtfProperty&> &proArr)
{
	CString temp;
	CString protemp;
	int count=arr.GetCount();
	int j=0;
	rtfProperty pro;//property struct
	for(int i=0;i<arr.GetCount();i++)
	{
		
		temp=arr.GetAt(i);
		memset(&pro,0,sizeof(rtfProperty));
		if(temp.Compare(_T("\\b"))==0)
		{
			if(proIsExist(temp,proArr)==-1)
			{
				pro.data=0;
				lstrcpy(pro.rtfType,temp.GetBuffer());
				temp.ReleaseBuffer();
				lstrcpy(pro.htmlTagB,_T("<b>"));
				lstrcpy(pro.htmlTagE,_T("</b>"));
				proArr.Add(pro);
			}
		}
		else if(temp.Compare(_T("\\b0"))==0)
		{
			int id=proIsExist(temp,proArr);
			if(id!=-1)
				proArr.RemoveAt(id);
		}
		else if(temp.Compare(_T("\\i"))==0)
		{
			if(proIsExist(temp,proArr)==-1)
			{
				pro.data=0;
				lstrcpy(pro.rtfType,temp.GetBuffer());
				temp.ReleaseBuffer();
				lstrcpy(pro.htmlTagB,_T("<i>"));
				lstrcpy(pro.htmlTagE,_T("</i>"));
				proArr.Add(pro);
			}
		}
		else if(temp.Compare(_T("\\i0"))==0)
		{
			int id=proIsExist(temp,proArr);
			if(id!=-1)
				proArr.RemoveAt(id);
		}
		else if(temp.Compare(_T("\\ul"))==0)
		{
			if(proIsExist(temp,proArr)==-1)
			{
				pro.data=0;
				lstrcpy(pro.rtfType,temp.GetBuffer());
				temp.ReleaseBuffer();
				lstrcpy(pro.htmlTagB,_T("<u>"));
				lstrcpy(pro.htmlTagE,_T("</u>"));
				proArr.Add(pro);
			}
		}
		else if(temp.Compare(_T("\\ulnono"))==0)
		{
			int id=proIsExist(temp,proArr);
			if(id!=-1)
				proArr.RemoveAt(id);
		}
		else if(temp.Compare(_T("\\strike"))==0)
		{
			if(proIsExist(temp,proArr)==-1)
			{
				pro.data=0;
				lstrcpy(pro.rtfType,temp.GetBuffer());
				temp.ReleaseBuffer();
				lstrcpy(pro.htmlTagB,_T("<strike>"));
				lstrcpy(pro.htmlTagE,_T("</strike>"));
				proArr.Add(pro);
			}
		}
		else if(temp.Compare(_T("\\strike0"))==0)
		{
			int id=proIsExist(temp,proArr);
			if(id!=-1);
				proArr.RemoveAt(id);
		}
		else if(temp.Find(_T("\\fs"))>=0)
		{
			int num=strGetNumber(temp);
			if(num<=0)
				num=24;
			int id=proIsExist(_T("\\fs"),proArr);
			if(id==-1)
			{
				pro.data=num;
				lstrcpy(pro.rtfType,_T("\\fs"));
				//temp.ReleaseBuffer();
				protemp.Format(_T("font-size:%d;"),num);
				lstrcpy(pro.htmlTagB,protemp.GetBuffer());
				protemp.ReleaseBuffer();
				pro.single=TRUE;
				proArr.Add(pro);
			}
			else{
				proArr.GetAt(id).data=num;
			}
		}
		else if(temp.Find(_T("\\cf"))>=0)   //颜色
		{
			int num=strGetNumber(temp);
			if(num==-1)
				continue;
			int id=proIsExist(_T("\\cf"),proArr);
			if(num>m_cg.GetCount())
				protemp.Format(_T("color:#%FFFFFF"));
			else if(num>=1)
				protemp.Format(_T("color:#%02x%02x%02x;"),GetRValue(m_cg.GetAt(num-1)),GetGValue(m_cg.GetAt(num-1)),GetBValue(m_cg.GetAt(num-1)));
			if(num==0 && id!=-1)
			{
				proArr.RemoveAt(id);
				//protemp.Format(_T("color:#%FF#FF#FF"));
			}
			else if(num>=1 &&  id!=-1) //修改
			{
				proArr.GetAt(id).data=num;
				lstrcpy(proArr.GetAt(id).htmlTagB,protemp.GetBuffer());
				protemp.ReleaseBuffer();
			}
			else if(id==-1) //添加
			{
				pro.data=num;
				lstrcpy(pro.rtfType,_T("\\cf"));
				//temp.ReleaseBuffer();
				lstrcpy(pro.htmlTagB,protemp.GetBuffer());
				protemp.ReleaseBuffer();
				pro.single=TRUE;
				proArr.Add(pro);
			}	
		}else if(temp.Find(_T("\\highlight"))>=0)   //颜色
		{
			int num=strGetNumber(temp);
			if(num==-1)
				continue;
			int id=proIsExist(_T("\\highlight"),proArr);
			if(num>m_cg.GetCount())
				protemp.Format(_T("color:#%FFFFFF"));
			else if(num>=1)
				protemp.Format(_T("color:#%02x%02x%02x;"),GetRValue(m_cg.GetAt(num-1)),GetGValue(m_cg.GetAt(num-1)),GetBValue(m_cg.GetAt(num-1)));
			if(num==0 && id!=-1)
			{
				proArr.RemoveAt(id);
			}else if(num>=1 &&  id!=-1) //修改
			{
				proArr.GetAt(id).data=num;
				lstrcpy(proArr.GetAt(id).htmlTagB,protemp.GetBuffer());
				protemp.ReleaseBuffer();
			}else if(id==-1) //添加
			{
				pro.data=num;
				lstrcpy(pro.rtfType,_T("\\highlight"));
				lstrcpy(pro.htmlTagB,protemp.GetBuffer());
				protemp.ReleaseBuffer();
				pro.single=TRUE;
				proArr.Add(pro);
			}	
		}

	}
}
int DLrtfhtml::proIsExist(CString str,CArray<rtfProperty,rtfProperty&> &proArr)
{
	for(int j=0;j<proArr.GetCount();j++)
	{
		if(str.Compare(proArr.GetAt(j).rtfType)==0)
			return j;
	}
	return -1; //not exist
}
//字符串分割为数组
void DLrtfhtml::stringSplit(CString source, CStringArray& dest, CString division)
{
    dest.RemoveAll();
    int pos = 0;
    int pre_pos = 0;
	while((pos=source.Find(division,pos))!=-1)
	{
		if(pos!=pre_pos)
			dest.Add(source.Mid(pre_pos,(pos-pre_pos)));
		pre_pos=pos;
		pos++;
	}
	if((source.GetLength()-1)>pre_pos)
		dest.Add(source.Mid(pre_pos));

}
long DLrtfhtml::strGetNumber(CString &str)//返回－1代表没有数字
{
	int i;
	int result;
	CString num;
	wchar_t* p;
	wchar_t temp;
	for(i=0;i<str.GetLength();i++)
	{
		if(isdigit(str.GetAt(i))!=0)
		{
			
			temp=str.GetAt(i);
			num+=temp;
		}
			
	}
	if(num.GetLength()>0)
		return _tcstol(num.GetBuffer(),&p,10);
	return -1;
}
int DLrtfhtml::getProPosition(CString &str,int start=0)
{
	if(start>=str.GetLength())
		return -1;
	while((start=str.Find(_T("\\"),start))>=0)
	{
		if(str.GetAt(start+1)=='\\')
			start+=2;
		else
			return start;
	}
	return -1;// 表示没找到
}
void DLrtfhtml::addHtmlTagToStr(CString &str,CArray<rtfProperty,rtfProperty&> &proArr)
{
	CString temp,tag,dtag;
	int i;
	for(i=0;i<proArr.GetCount();i++)
	{//添加单标记属性
		if(proArr.GetAt(i).single==TRUE)
		{
			tag+=proArr.GetAt(i).htmlTagB;
			tag+=_T(" ");
			//tag.Format(_T(" %s %s"),tag,proArr.GetAt(i).htmlTagB);
		}
	}
	//添加双标记
	dtag=str;
	for(i=0;i<proArr.GetCount();i++)
	{
		if(proArr.GetAt(i).single!=TRUE)
		{
			dtag.Format(_T("%s%s%s"),proArr.GetAt(i).htmlTagB,dtag,proArr.GetAt(i).htmlTagE);
		}
	}
	if(!tag.IsEmpty())
	{
		temp.Format(_T("<span style=\"%s\" >"),tag);
		temp+=dtag;
		temp+=_T("</span>");
		str=temp;
	}
	else
		str=dtag;
	
}
// DLrtfhtml 成员函数
#pragma once

// DLrtfhtml 命令目标

typedef struct fontgroup
{
	int fid;//字体编号
	int fprq;//间隔
	TCHAR fname[20];//字体名
	
}*pfontg;
struct rtfProperty
{	
	TCHAR rtfType[20];  //rtf标记
	TCHAR htmlTagB[40]; //html开始标记
	TCHAR htmlTagE[40];
	BOOL single;//为真，为非闭合
	int data;	//数据
};
class DLrtfhtml : public CObject
{
public:
	DLrtfhtml();
	virtual ~DLrtfhtml();
	CString destcon;//内容
	CArray<fontgroup,fontgroup&> m_fg;//字体数组
	CArray<COLORREF,COLORREF&> m_cg;//颜色组
	CArray<rtfProperty,rtfProperty&> m_defaultPro;//默认属性
	CArray<rtfProperty,rtfProperty&> m_curPro;//当前属性
	
	CString beginChange(CString rtfPath);
	void openfile(CString filename);
	void stringSplit(CString source, CStringArray& dest, CString division);//字符串分割为数组
	BOOL readcolor();
	CString toHtmlTag();
private:
	void readfontgroup();//得到字体组
	void getDefaultProperty();//得到默认属性
	void addProToArr(CStringArray &arr,CArray<rtfProperty,rtfProperty&> &proArr);//添加属性到数组
	int  proIsExist(CString str,CArray<rtfProperty,rtfProperty&> &proArr);//查看属性是否存在
	long strGetNumber(CString &str);//从字符串中获取数字
	int getProPosition(CString &str,int start);  //得到字符串属性的起始位置
	void addHtmlTagToStr(CString &str,CArray<rtfProperty,rtfProperty&> &proArr); //给字符串添加属性
};
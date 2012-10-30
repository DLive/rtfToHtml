#pragma once

// DLrtfhtml ����Ŀ��

typedef struct fontgroup
{
	int fid;//������
	int fprq;//���
	TCHAR fname[20];//������
	
}*pfontg;
struct rtfProperty
{	
	TCHAR rtfType[20];  //rtf���
	TCHAR htmlTagB[40]; //html��ʼ���
	TCHAR htmlTagE[40];
	BOOL single;//Ϊ�棬Ϊ�Ǳպ�
	int data;	//����
};
class DLrtfhtml : public CObject
{
public:
	DLrtfhtml();
	virtual ~DLrtfhtml();
	CString destcon;//����
	CArray<fontgroup,fontgroup&> m_fg;//��������
	CArray<COLORREF,COLORREF&> m_cg;//��ɫ��
	CArray<rtfProperty,rtfProperty&> m_defaultPro;//Ĭ������
	CArray<rtfProperty,rtfProperty&> m_curPro;//��ǰ����
	
	CString beginChange(CString rtfPath);
	void openfile(CString filename);
	void stringSplit(CString source, CStringArray& dest, CString division);//�ַ����ָ�Ϊ����
	BOOL readcolor();
	CString toHtmlTag();
private:
	void readfontgroup();//�õ�������
	void getDefaultProperty();//�õ�Ĭ������
	void addProToArr(CStringArray &arr,CArray<rtfProperty,rtfProperty&> &proArr);//������Ե�����
	int  proIsExist(CString str,CArray<rtfProperty,rtfProperty&> &proArr);//�鿴�����Ƿ����
	long strGetNumber(CString &str);//���ַ����л�ȡ����
	int getProPosition(CString &str,int start);  //�õ��ַ������Ե���ʼλ��
	void addHtmlTagToStr(CString &str,CArray<rtfProperty,rtfProperty&> &proArr); //���ַ����������
};
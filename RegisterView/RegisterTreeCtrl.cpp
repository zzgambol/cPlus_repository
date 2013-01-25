// RegisterTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "RegisterTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegisterTreeCtrl

CRegisterTreeCtrl::CRegisterTreeCtrl()
{
	m_uFlags = 0;
}

CRegisterTreeCtrl::~CRegisterTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CRegisterTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CRegisterTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegisterTreeCtrl message handlers
const HKEY hRoot[] = {HKEY_CLASSES_ROOT,
					  HKEY_CURRENT_USER,					  
					  HKEY_LOCAL_MACHINE,
					  HKEY_USERS,
					  HKEY_CURRENT_CONFIG,
					  HKEY_PERFORMANCE_DATA,
					  HKEY_DYN_DATA
};

const char* strRoot[] = {"HKEY_CLASSES_ROOT",
						 "HKEY_CURRENT_USER",						 
						 "HKEY_LOCAL_MACHINE",
						 "HKEY_USERS",
						 "HKEY_CURRENT_CONFIG",
						 "HKEY_PERFORMANCE_DATA",
						 "HKEY_DYN_DATA"
};

BOOL CRegisterTreeCtrl::DisplayTree(BOOL bWinNT)
{
	DeleteAllItems();

	DisplayDrives(bWinNT);

	return TRUE;	
}

BOOL CRegisterTreeCtrl::DisplayDrives(BOOL bWinNT)
{
	UINT nSize = sizeof(hRoot)/sizeof(hRoot[0]);
	UINT nCycle = 0;
	while( nCycle < nSize )
	{
		if(IsExist(hRoot[nCycle],NULL))
		{
			HTREEITEM hParent = AddItem( TVI_ROOT, strRoot[nCycle] ,hRoot[nCycle]);
			if ( FindSubDir( hRoot[nCycle],NULL ) )
				InsertItem( "", 0, 0, hParent );
		}
		nCycle++;
	}

	return TRUE;
}

BOOL CRegisterTreeCtrl::IsExist(HKEY hKey,LPCTSTR lpSubKey)
{
	HKEY phkResult;
	if( ERROR_SUCCESS == RegOpenKeyEx(hKey,lpSubKey,0,
		KEY_EXECUTE,&phkResult))
	{
		RegCloseKey(phkResult);
		return TRUE;
	}
	return FALSE;
}

HTREEITEM CRegisterTreeCtrl::AddItem(HTREEITEM hParent, LPCTSTR strPath, HKEY hKey)
{
    CString    strTemp = strPath;
	HTREEITEM hReturn;

	if ( hParent == TVI_ROOT )
	{
		hReturn = InsertItem(TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_STATE,
			(LPCTSTR)strTemp,0,1,INDEXTOSTATEIMAGEMASK(1),
			TVIS_STATEIMAGEMASK,0,hParent,TVI_LAST);
		SetItemData(hReturn,(DWORD)hKey);
		return hReturn;
	}
	
	hReturn = InsertItem(TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_STATE,
		(LPCTSTR)strTemp,0,1,INDEXTOSTATEIMAGEMASK(1),
		TVIS_STATEIMAGEMASK,0,hParent,TVI_LAST);
	SetItemData(hReturn,(DWORD)hKey);
	return hReturn;
}

BOOL CRegisterTreeCtrl::FindSubDir(HKEY hKey,LPCTSTR lpSubKey)
{
	if(IsExist(hKey,lpSubKey))
	{
		HKEY phkResult;
		if( ERROR_SUCCESS == RegOpenKeyEx(hKey,lpSubKey,0,
			KEY_READ,&phkResult))
		{
			char sz[MAX_PATH];
			if( ERROR_SUCCESS == RegEnumKey(phkResult,0,sz,MAX_PATH))
			{
				RegCloseKey(phkResult);
				return TRUE;
			}
			RegCloseKey(phkResult);
		}
	}
	return FALSE;
}

void CRegisterTreeCtrl::ExpandItem(HTREEITEM hItem, UINT nCode)
{	
	CString strSubKey;
	
	if ( nCode == TVE_EXPAND )
	{
		HTREEITEM hChild = GetChildItem( hItem );
		while ( hChild )
		{
			DeleteItem( hChild );
			hChild = GetChildItem( hItem );
		}
        
		HKEY hKey = (HKEY)GetItemData(hItem);
		strSubKey = GetSubKey(hItem);
		if(strSubKey.IsEmpty())
			DisplayPath( hItem, NULL ,hKey );
		else
			DisplayPath( hItem, strSubKey ,hKey );
	}
}

void CRegisterTreeCtrl::DisplayPath(HTREEITEM hParent, LPCTSTR lpSubKey , HKEY hKey)
{
	HKEY phkResult;
	char sz[MAX_PATH];
	DWORD nReturn;
	CString strSubKey = lpSubKey;
	CString strName;

	if( ERROR_SUCCESS != RegOpenKeyEx(hKey,lpSubKey,0,
		KEY_READ,&phkResult))
	{
		return;
	}

	CWaitCursor wc;
	SetRedraw( FALSE );
	for(int i=0 ; ; i++)
	{
		nReturn = RegEnumKey(phkResult,i,sz,MAX_PATH);
		if( nReturn != ERROR_SUCCESS )
			break;
		HTREEITEM hItem = AddItem( hParent , sz , hKey);
		strName.Format("%s",sz);
		if(lpSubKey == NULL)
		{
			strSubKey = strName;
		}
		else
		{
			strSubKey.Format("%s",lpSubKey);
			strSubKey += "\\" + strName;
		}
		if (FindSubDir(hKey , strSubKey))
			InsertItem( "", 0, 0, hItem );
	}
	RegCloseKey(phkResult);

	//nState: 0->无选择钮 1->没有选择 2->部分选择 3->全部选择
	UINT nState = GetItemState( hParent, TVIS_STATEIMAGEMASK ) >> 12;
	if(nState == 3)
		SetItemState( hParent, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );

	SetRedraw( TRUE );	
}

void CRegisterTreeCtrl::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CString strPath;
	 
	if ( pNMTreeView->itemNew.state & TVIS_EXPANDED )
	{
		ExpandItem( pNMTreeView->itemNew.hItem, TVE_EXPAND );
	}
	else
	{
		HTREEITEM hChild = GetChildItem( pNMTreeView->itemNew.hItem );
				
		while ( hChild ) 
		{
			DeleteItem( hChild );
			hChild = GetChildItem( pNMTreeView->itemNew.hItem );
		}
		InsertItem( "", pNMTreeView->itemNew.hItem );
	}
	*pResult = 0;
}

CString CRegisterTreeCtrl::GetSubKey(HTREEITEM hItem)
{
	CString strReturn;
	CString strTemp;
	int nIndex = 0;
	HTREEITEM hParent = hItem;

	strReturn = "";

	while ( hParent )
	{		
		strTemp  = GetItemText( hParent );
		strTemp += "\\";
		strReturn = strTemp + strReturn;
		hParent = GetParentItem( hParent );
	}
    
	strReturn.TrimRight( '\\' );

	nIndex = strReturn.Find("\\");
	if(nIndex == -1)
		strReturn = "";
	else
		strReturn = strReturn.Right(strReturn.GetLength()-nIndex-1);

    return strReturn;
}

BOOL CRegisterTreeCtrl::SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearch)
{
	BOOL bReturn = CTreeCtrl::SetItemState( hItem, nState, nStateMask );

	UINT iState = nState >> 12;
	if(iState!=0)
	{
		if(bSearch) TravelChild(hItem, iState);
		TravelSiblingAndParent(hItem,iState);
	}
	return bReturn;
}

void CRegisterTreeCtrl::TravelChild(HTREEITEM hItem, int nState)
{
	HTREEITEM hChildItem,hBrotherItem;
	
	//查找子节点，没有就结束
	hChildItem=GetChildItem(hItem);
	if(hChildItem!=NULL)
	{
		//设置子节点的状态与当前节点的状态一致
		CTreeCtrl::SetItemState( hChildItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
		//再递归处理子节点的子节点和兄弟节点
		TravelChild(hChildItem, nState);
		
		//处理子节点的兄弟节点和其子节点
		hBrotherItem=GetNextSiblingItem(hChildItem);
		while (hBrotherItem)
		{
			//设置子节点的兄弟节点状态与当前节点的状态一致
			int nState1 = GetItemState( hBrotherItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=0)
			{
				CTreeCtrl::SetItemState( hBrotherItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
			}
			//再递归处理子节点的兄弟节点的子节点和兄弟节点
			TravelChild(hBrotherItem, nState);
			hBrotherItem=GetNextSiblingItem(hBrotherItem);
		}
	}
}

void CRegisterTreeCtrl::TravelSiblingAndParent(HTREEITEM hItem, int nState)
{
	HTREEITEM hNextSiblingItem,hPrevSiblingItem,hParentItem;
	
	//查找父节点，没有就结束
	hParentItem=GetParentItem(hItem);
	if(hParentItem!=NULL)
	{
		int nState1=nState;//设初始值，防止没有兄弟节点时出错
		
		//查找当前节点下面的兄弟节点的状态
		hNextSiblingItem=GetNextSiblingItem(hItem);
		while(hNextSiblingItem!=NULL)
		{
			nState1 = GetItemState( hNextSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=nState && nState1!=0) break;
			else hNextSiblingItem=GetNextSiblingItem(hNextSiblingItem);
		}
		
		if(nState1==nState)
		{
			//查找当前节点上面的兄弟节点的状态
			hPrevSiblingItem=GetPrevSiblingItem(hItem);
			while(hPrevSiblingItem!=NULL)
			{
				nState1 = GetItemState( hPrevSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
				if(nState1!=nState && nState1!=0) break;
				else hPrevSiblingItem=GetPrevSiblingItem(hPrevSiblingItem);
			}
		}
		
		if(nState1==nState || nState1==0)
		{
			nState1 = GetItemState( hParentItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=0)
			{
				//如果状态一致，则父节点的状态与当前节点的状态一致
				CTreeCtrl::SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
			}
			//再递归处理父节点的兄弟节点和其父节点
			TravelSiblingAndParent(hParentItem,nState);
		}
		else
		{
			//状态不一致，则当前节点的父节点、父节点的父节点……状态均为第三态
			hParentItem=GetParentItem(hItem);
			while(hParentItem!=NULL)
			{
				nState1 = GetItemState( hParentItem, TVIS_STATEIMAGEMASK ) >> 12;
				if(nState1!=0)
				{
					CTreeCtrl::SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );
				}
				hParentItem=GetParentItem(hParentItem);
			}
		}
	}	
}

void CRegisterTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_uFlags & TVHT_ONITEMSTATEICON)
		*pResult=1;
	else 
		*pResult = 0;
}

void CRegisterTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	HTREEITEM hItem = HitTest(point, &m_uFlags);
	if ( (m_uFlags&TVHT_ONITEMSTATEICON ))
	{
		//nState: 0->无选择钮 1->没有选择 2->部分选择 3->全部选择
		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
		nState=(nState==3)?1:3;
		SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
	}
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CRegisterTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
 	//处理空格键
 	if(nChar==0x20)
 	{
 		HTREEITEM hItem = GetSelectedItem();
 		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
 		if(nState!=0)
 		{
 			nState=(nState==3)?1:3;
			SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
 		}
 	}
	else	
		CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRegisterTreeCtrl::CalcuSelectPath()
{
	UINT nState;
	HTREEITEM hParent;
	CString strPath;

	m_strlist.RemoveAll();
	hParent = GetRootItem();
	while( hParent != NULL )
	{
		nState = GetItemState( hParent, TVIS_STATEIMAGEMASK ) >> 12;
		switch(nState)
		{
		case 2:
			strPath = GetItemText(hParent);
			CalcuSelectSubPath(hParent,strPath);
			break;
		case 3:
			strPath = GetItemText(hParent);
			//strPath += "\\";
			m_strlist.AddTail(strPath);
			break;
		}
		hParent = GetNextSiblingItem(hParent);
	};	
}

void CRegisterTreeCtrl::CalcuSelectSubPath(HTREEITEM hParent,CString strPath)
{
	UINT nState;
	HTREEITEM hItem = GetChildItem(hParent);
	CString strPathName;
	CString strDirectory(strPath);
	CString strTemp;
	while( hItem != NULL )
	{
		nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
		switch(nState)
		{
		case 2:
			strPathName = GetItemText(hItem);
			strTemp = strDirectory;
			strTemp += "\\" + strPathName;
			CalcuSelectSubPath(hItem,strTemp);
			break;
		case 3:
			strPathName = GetItemText(hItem);
			strTemp = strDirectory;
			strTemp += "\\" + strPathName;
			m_strlist.AddTail(strTemp);
			break;
		}
		hItem = GetNextSiblingItem(hItem);
	};
}

CStringList* CRegisterTreeCtrl::GetSelectKey()
{
	CalcuSelectPath();
	if(m_strlist.IsEmpty())
		return NULL;
	return &m_strlist;
}

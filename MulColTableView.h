//
//  MulColTableView.h
//  PaoPaoTang
//
//  Created by Sheng on 12/17/13.
//
//https://github.com/aboduo/MulColTableView

#ifndef __PaoPaoTang__MulColTableView__
#define __PaoPaoTang__MulColTableView__

#include "cocos2d.h"
#include <extensions/cocos-ext.h>




//#include "CCTableView.h"

NS_CC_EXT_BEGIN


class MulColTableView;

class MulColTableViewDelegate : public TableViewDelegate
{
public:
	
	virtual void tableCellTouched(TableView* table, TableViewCell* cell) = 0;
	//virtual void tableCellTouched(TableView* table, TableViewCell* cell){};
    
    virtual void tableCellTouched(MulColTableView* table, Node* cell, int row, int column) = 0;
    virtual void tableCellHighlight(MulColTableView* table, Node* cell, int row, int column){};
    virtual void tableCellUnhighlight(MulColTableView* table, Node* cell, int row, int column){};

	

	
	
	
//    virtual void tableCellWillRecycle(MulColTableView* table, CCNode* cell){};
	
	virtual void tableCellHighlight(TableView* table, TableViewCell* cell){};
	virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell){};
	virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){};

	
};



/**
 * Data source that governs table backend data.
 */
class MulColTableViewDataSource 
{
public:
    
    virtual Size cellSizeForTable(MulColTableView *table) = 0;

    virtual Node* tableCellAt(MulColTableView *table, unsigned int row,  unsigned int column) = 0;

    virtual unsigned int numberOfRowsInTableView(MulColTableView *table) = 0;
    virtual unsigned int numberOfColsInTableView(MulColTableView *table) = 0;
};


class MulColTableView : public TableView, public TableViewDataSource
{
    
protected:
    MulColTableViewDelegate*        m_pMulColDelegate;
    MulColTableViewDataSource*      m_pMulColDataSource;
    
    int    m_nTouchCol;   //// used for touch
    int    m_nTouchRow;
    
    int    m_nColCount;
    
public:
    MulColTableView()
    :m_nTouchCol(-1),
    m_nTouchRow(-1)
    {
        m_pMulColDelegate = NULL;
        m_pMulColDataSource = NULL;
    };
    
    static MulColTableView* create(MulColTableViewDataSource* dataSource, Size size);
    static MulColTableView* create(MulColTableViewDataSource* dataSource, Size size, Node *container);
    
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
    //// CCTableViewDataSource
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    /**
     *  multi delegate
     */
    MulColTableViewDelegate* getMulColDelegate() { return m_pMulColDelegate; }
    void setMulColDelegate(MulColTableViewDelegate* pDelegate)
    {
        m_pMulColDelegate = pDelegate;
        /*CCTableView::setDelegate(pDelegate);*/
    }
    
    void setMulColDataSource(MulColTableViewDataSource* pDataSource){ m_pMulColDataSource = pDataSource; }
    
    
    virtual void tableCellTouched(TableView* table, TableViewCell* cell){};
//    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
    virtual void scrollViewDidScroll(ScrollView* view);
    virtual void scrollViewDidZoom(ScrollView* view){};
};

NS_CC_EXT_END



#endif /* defined(__PaoPaoTang__MulColTableView__) */

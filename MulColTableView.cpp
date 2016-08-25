//
//  MulColTableView.cpp
//  PaoPaoTang
//
//  Created by Sheng on 12/17/13.
//
//

#include "MulColTableView.h"

NS_CC_EXT_BEGIN

#define kColCell_Tag       1180


MulColTableView* MulColTableView::create(MulColTableViewDataSource* dataSource, Size size)
{
    return MulColTableView::create(dataSource, size, NULL);
}

MulColTableView* MulColTableView::create(MulColTableViewDataSource* dataSource, Size size, Node *container)
{
    MulColTableView *table = new (std::nothrow) MulColTableView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setMulColDataSource(dataSource);
    table->setDataSource(table);
//    table->setDelegate(table);
    table->_updateCellPositions();
    table->_updateContentSize();
    
//    table->setDirection(kCCScrollViewDirectionVertical);
//    table->setVerticalFillOrder(kCCTableViewFillTopDown);
    
    return table;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark - CCTouchDelegate

void MulColTableView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible()) {
        return;
    }
    
    if (_touchedCell){
		Rect bb = this->boundingBox();
		bb.origin = _parent->convertToWorldSpace(bb.origin);
        
		if ( bb.containsPoint( pTouch->getLocation() ) && m_pMulColDelegate != NULL)
        {
            Node* multiCell = _touchedCell->getChildByTag(kColCell_Tag+m_nTouchCol);
            if (multiCell)
            {
                m_pMulColDelegate->tableCellUnhighlight(this, multiCell, m_nTouchRow, m_nTouchCol);
                m_pMulColDelegate->tableCellTouched(this, multiCell, m_nTouchRow, m_nTouchCol);
            }

        }
        
        _touchedCell = NULL;
        m_nTouchRow = -1;
        m_nTouchCol = -1;
    }
    
    ScrollView::onTouchEnded(pTouch, pEvent);
    
    
}



bool MulColTableView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible()) {
        return false;
    }
    
    bool touchResult = ScrollView::onTouchBegan(pTouch, pEvent);
    
    Point           point;
    
    point = this->getContainer()->convertTouchToNodeSpace(pTouch);
    m_nTouchRow = this->_indexFromOffset(point);
    
    if(_touches.size() == 1) {
        
        const Size cellSize = m_pMulColDataSource->cellSizeForTable(this);
        
        m_nTouchCol = point.x / cellSize.width;
        
		if (m_nTouchRow == CC_INVALID_INDEX)
		{
			_touchedCell = NULL;
		}
        else
		{
			_touchedCell  = this->cellAtIndex(m_nTouchRow);
		}
        
        if (_touchedCell && m_pMulColDelegate != NULL) {
            
            Node* multiCell = _touchedCell->getChildByTag(kColCell_Tag+m_nTouchCol);
            if (multiCell)
            {
                m_pMulColDelegate->tableCellHighlight(this, multiCell, m_nTouchRow, m_nTouchCol);
            }
        }
    }
    else if(_touchedCell)
    {
        if(m_pMulColDelegate != NULL)
        {
            Node* multiCell = _touchedCell->getChildByTag(kColCell_Tag+m_nTouchCol);
            if (multiCell)
            {
                m_pMulColDelegate->tableCellUnhighlight(this, multiCell, m_nTouchRow, m_nTouchCol);
            }
        }
        
        _touchedCell = NULL;
        m_nTouchRow = -1;
        m_nTouchCol = -1;
    }
    
    return touchResult;
    
}



void MulColTableView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchMoved(pTouch, pEvent);
    
    if (_touchedCell && isTouchMoved()) {
        if(m_pMulColDelegate != NULL) {
            Node* multiCell = _touchedCell->getChildByTag(kColCell_Tag+m_nTouchCol);
            if (multiCell)
            {
                m_pMulColDelegate->tableCellUnhighlight(this, multiCell, m_nTouchRow, m_nTouchCol);
            }
            
        }
        
        _touchedCell = NULL;
    }
}


void MulColTableView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchCancelled(pTouch, pEvent);
    
    if (_touchedCell) {
        if(m_pMulColDelegate != NULL) {
            Node* multiCell = _touchedCell->getChildByTag(kColCell_Tag+m_nTouchCol);
            if (multiCell)
            {
                m_pMulColDelegate->tableCellUnhighlight(this, multiCell, m_nTouchRow, m_nTouchCol);
            }
            
        }
        
        _touchedCell = NULL;
        m_nTouchRow = -1;
        m_nTouchCol = -1;
    }
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark - CCTableViewDelegate


//void MulColTableView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
//{
//    int c = m_pMulColDataSource->numberOfColsInTableView(this);
//    
//    for (int i = 0; i < c ; i++)
//    {
//        
//        CCNode* multiCell = cell->getChildByTag(kColCell_Tag+i);
//        if (m_pMulColDelegate)
//        {
//            m_pMulColDelegate->tableCellWillRecycle(this,multiCell);
//        }
//        cell->removeChildByTag(kColCell_Tag+i, true);
//        
//    }
//}

void MulColTableView::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
    TableView::scrollViewDidScroll(view);
    
    if (m_pMulColDelegate)
    {
        m_pMulColDelegate->scrollViewDidScroll(view);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark - CCTableViewDataSource

Size MulColTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    int height = m_pMulColDataSource->cellSizeForTable(this).height;
    int width = m_pMulColDataSource->cellSizeForTable(this).width * m_pMulColDataSource->numberOfColsInTableView(this);
    
    return Size(width, height);

}


TableViewCell* MulColTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = this->dequeueCell();
    if (!cell) {
        cell = new TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    int c = m_pMulColDataSource->numberOfColsInTableView(this);
    
    
    for (int i = 0; i < c ; i++)
    {
//        cell->removeChildByTag(kColCell_Tag+i, true);
        
        Node* colCell = m_pMulColDataSource->tableCellAt(this, idx, i);
        
        if (colCell)
        {
            colCell->setTag(kColCell_Tag+i);
            
            Size size = m_pMulColDataSource->cellSizeForTable(this);
            colCell->setAnchorPoint(Point(0.5, 0.5));
            colCell->setPosition(size.width * (i+0.5), size.height/2);
            cell->addChild(colCell);
        }

    }
    
    return cell;
}


ssize_t MulColTableView::numberOfCellsInTableView(TableView *table)
{
    int count = m_pMulColDataSource->numberOfRowsInTableView(this);
    return count;
}



NS_CC_EXT_END
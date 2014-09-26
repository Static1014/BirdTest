//
//  PopListLayer.h
//  BirdTest
//
//  Created by XiongJian on 14-9-25.
//
//

#ifndef __BirdTest__PopListLayer__
#define __BirdTest__PopListLayer__

#include "Constance.h"
#include "PopLayer.h"

class PopListLayer : public PopLayer ,public TableViewDataSource, public TableViewDelegate {
public:
    PopListLayer();
    ~PopListLayer();

    virtual bool init();
    CREATE_FUNC(PopListLayer);

    void onEnter();
    void initTableView();

private:
    TableView* tableView;

    virtual void scrollViewDidScroll(ScrollView* view){};
    virtual void scrollViewDidZoom(ScrollView* view){}

    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);

    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    void clickItem(ssize_t idx);

    virtual ssize_t numberOfCellsInTableView(TableView *table);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);

    void touchUpInsideAction(Ref *sender, Control::EventType controlEvent);
};

#endif /* defined(__BirdTest__PopListLayer__) */

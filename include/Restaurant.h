#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"




class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    std::vector <Table*> getTables();
    void PushAction(BaseAction  &action);
    virtual ~Restaurant();//Destructor
    Restaurant(const Restaurant& other );//Copy constructor
    Restaurant& operator=(const Restaurant& other);//Copy assignment operator
    Restaurant(Restaurant && other);//Move constructor
    Restaurant& operator=(Restaurant && other);//Move assignment operator

private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif

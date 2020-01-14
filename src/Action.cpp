#include "../include/Action.h"
//#include "../include/Table.h"
#include "../include/Restaurant.h"
#include <iostream>
#include <sstream>

extern Restaurant* backup;

BaseAction::BaseAction() {
    errorMsg="";
    status=PENDING;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    this->status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    this->status = ERROR;
    this->errorMsg = errorMsg;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : tableId(id) ,customers(customersList)   {
    logString="";
    for(int i=0;(unsigned)i<customers.size();i++)  
        logString=logString+customers[i]->getName() + "," + customers[i]->getStrategy()+" ";
}

OpenTable::~OpenTable() {
  }
//Openning a table with a given id and assigns customers to it

void OpenTable::act(Restaurant &restaurant) {
  //Checking if table exists
    if (restaurant.getTable(this->tableId) == nullptr){
        this->error("Table does not exist or is already open");
        for(Customer *c:customers){
            delete c;
        }
        customers.clear();
    }
    //Checking if table is already open
    else if (restaurant.getTable(this->tableId)->isOpen()) {
        this->error("Table does not exist or is already open");
        for(Customer *c:customers){
            delete c;
        }
        customers.clear();
    }
    else {
      //Openning the table
        Table * table= restaurant.getTable(tableId);
        table->openTable();
        for (int i = 0;(unsigned) i < customers.size(); i++)
        {
            table->addCustomer(customers[i]);
        }
        complete();

    }
    //Pushing action to actionslog vector
    restaurant.PushAction(*this);
    if(this->getStatus()==ERROR)
        std::cout<< "Error: "<<this->getErrorMsg()<<std::endl;
}

std::string OpenTable::toString() const {
    std::string s = "open " + std::to_string(tableId) + " ";
        s = s + logString;
    if (getStatus() == COMPLETED)
        s = s   + "Completed";
    if (getStatus() == ERROR)
        s = s  + "Error:" + " " + this->getErrorMsg();
    return s;
}

Order::Order(int id) : tableId(id) {}

void Order::act(Restaurant &restaurant) {
    restaurant.PushAction(*this);
    std::string orderPrint = "";
    std::vector<int> orders;
    if (restaurant.getTable(this->tableId) == nullptr)
        this->error("Table does not exist or is not open");
    else if (restaurant.getTable(this->tableId)->isOpen() == false)
        this->error("Table does not exist or is not open");
    else {
        Table *t = restaurant.getTable(tableId);
        t->order(restaurant.getMenu());
        this->complete();
    }
    if(this->getStatus()==ERROR)
        std::cout<< "Error: "<<this->getErrorMsg()<<std::endl;
}

std::string Order::toString() const {
    std::string s = "order " + std::to_string(tableId);
    if (getStatus() == COMPLETED)
        s = s + " " + "Completed";
    if (getStatus() == ERROR)
        s = s + " " + "Error:" + " " + this->getErrorMsg();
    return s;
}

Close::Close(int id) : tableId(id) {}

//Closing table
void Close::act(Restaurant &restaurant) {
    restaurant.PushAction(*this);
    if (restaurant.getTable(this->tableId) == nullptr)
        this->error("Table does not exist or is already open");
    else if (restaurant.getTable(this->tableId)->isOpen() == false)
        this->error("Table does not exist or is not open");
    else {
        std::cout << "Table " << tableId << " was closed. Bill " << restaurant.getTable(tableId)->getBill() << "NIS"
                  << std::endl;
        restaurant.getTable(tableId)->closeTable();
        complete();
    }
    if(this->getStatus()==ERROR)
        std::cout<< "Error: "<<this->getErrorMsg()<<std::endl;
}

std::string Close::toString() const {
    std::string s = "close " + std::to_string(tableId);
    if (getStatus() == COMPLETED)
        s = s + " " + "Completed";
    if (getStatus() == ERROR)
        s = s + " " + "Error:" + " " + this->getErrorMsg();
    return s;

}

CloseAll::CloseAll() {}

std::string CloseAll::toString() const {
    return "closeall Completed";
}

void CloseAll::act(Restaurant &restaurant) {
    restaurant.PushAction(*this);
    std::string s = "";
    std::vector<Table*> tables=restaurant.getTables();
    for (int i = 0;(unsigned) i < tables.size(); i++) {
        Table* table=tables[i];
	//Checking if table is open in tables vector, if it is open - close it.
        if (table->isOpen()) {
            std::cout << "Table " << i << " was closed. Bill " << table->getBill() << "NIS"
                      << std::endl;
            table->closeTable();
        }
    }
    complete();

}

PrintMenu::PrintMenu() {}

std::string PrintMenu::toString() const {
    return "menu Completed";
}

void PrintMenu::act(Restaurant &restaurant) {
    restaurant.PushAction(*this);
    std::string type = "";
    //Checking customer type and converts it to string for the printing.
    for (int i = 0;(unsigned) i < restaurant.getMenu().size(); i++) {
        if (restaurant.getMenu()[i].getType() == VEG)
            type = "VEG";
        if (restaurant.getMenu()[i].getType() == SPC)
            type = "SPC";
        if (restaurant.getMenu()[i].getType() == ALC)
            type = "ALC";
        if (restaurant.getMenu()[i].getType() == BVG)
            type = "BVG";
        std::cout << restaurant.getMenu()[i].getName() << " " <<type << " "
                  << restaurant.getMenu()[i].getPrice() << "NIS" << std::endl;
    }
    complete();
}

PrintTableStatus::PrintTableStatus(int id) : tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant) {
    restaurant.PushAction(*this);
    std::vector<Table *> tables = restaurant.getTables();
        if (tables[tableId]->isOpen() == false)
            std::cout << "Table " << tableId << " status: closed" << std::endl;
        else {
            std::cout << "Table " << tableId << " status: open" << std::endl;
            std::cout << "Customers:" << std::endl;
            std::vector<Customer *> customers = tables[tableId]->getCustomers();
            for (int j = 0;(unsigned) j < customers.size(); j++)
                std::cout << customers[j]->getId() << " " << customers[j]->getName() << std::endl;
            std::cout << "Orders:" << std::endl;
            std::vector<OrderPair> orders = tables[tableId]->getOrders();
            for (int k = 0;(unsigned) k < orders.size(); k++) {
                Dish dish = orders[k].second;
                std::cout << dish.getName() << " " << dish.getPrice() << "NIS" << " " << orders[k].first << std::endl;
            }
            std::cout << "Current Bill: " << tables[tableId]->getBill() << "NIS" << std::endl;

    }
    complete();
}

std::string PrintTableStatus::toString() const {
    return "status " + std::to_string(tableId) + " Completed";
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTable(src), dstTable(dst), id(customerId) {}

std::string MoveCustomer::toString() const {
    std::string s = "";
    s = s + "move " + std::to_string(srcTable) + " " + std::to_string(dstTable) + " " + std::to_string(id);
    if (getStatus() == COMPLETED)
        s = s + " " + "Completed";
    if (getStatus() == ERROR)
        s = s + " " + "Error:" + " " + this->getErrorMsg();
    return s;

}

void MoveCustomer::act(Restaurant &restaurant) {
    restaurant.PushAction(*this);
    //Checking if we can move the customer to the destination table.
    if (restaurant.getTable(srcTable)->isOpen() == false || restaurant.getTable(dstTable)->isOpen() == false)
        this->error("Cannot move customer");
        else if (restaurant.getTable(srcTable)->getCustomer(id) == nullptr)
            this->error("Cannot move customer");
       else if (restaurant.getTable(dstTable)->getCustomers().size() >= (unsigned) restaurant.getTable(dstTable)->getCapacity())
            this->error("Cannot move customer");

    else {
      //We can move the customer to destination table.
        std::vector<OrderPair> orders = restaurant.getTable(srcTable)->getOrders();
        for (int i = 0;(unsigned) i < orders.size(); i++) {
	  if (orders[i].first == id) {//Checking which orders the client that is about to move,ordered.
	      restaurant.getTable(dstTable)->getOrders().push_back(restaurant.getTable(srcTable)->getOrders()[i]);//Adding order to the destination table.
	      restaurant.getTable(dstTable)->setBill(restaurant.getTable(dstTable)->getBill()+orders[i].second.getPrice());//Updating destination table's bill.
	      restaurant.getTable(srcTable)->setBill(restaurant.getTable(srcTable)->getBill()-orders[i].second.getPrice());//Updating source table's bill.

            }
        }
        std::vector<OrderPair> copy(restaurant.getTable(srcTable)->getOrders());//Making a copy of the source table's orders.
        restaurant.getTable(srcTable)->getOrders().clear();//Deleting all orders in original source table.
        for (int i = 0;(unsigned) i < copy.size(); i++)
	  if (copy[i].first != id)//Copying all orders that weren't ordered by the customer that have been moved.
                restaurant.getTable(srcTable)->getOrders().push_back(copy[i]);

        restaurant.getTable(dstTable)->getCustomers().push_back(restaurant.getTable(srcTable)->getCustomer(id));//Pushing new customer to destination table.
        for (int i = 0;(unsigned) i < restaurant.getTable(srcTable)->getCustomers().size(); i++)
            if (id == restaurant.getTable(srcTable)->getCustomers()[i]->getId())
	      restaurant.getTable(srcTable)->removeCustomer(id);//Removing customer from source table.
        if (restaurant.getTable(srcTable)->getCustomers().size() == 0)
            restaurant.getTable(srcTable)->closeTable();
        complete();
    }
    if(this->getStatus()==ERROR)
        std::cout<< "Error: "<<this->getErrorMsg()<<std::endl;

}

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Restaurant &restaurant) {
  //Prints all action's toString.
    std::string s = "";
    std::vector<BaseAction*> logs=restaurant.getActionsLog();
    for (int i = 0;(unsigned) i < logs.size()-1; i++) {

        s = s+logs[i]->toString()  + "\n";
    }
    s=s+logs[logs.size()-1]->toString();
    std::cout << s << std::endl;
    restaurant.PushAction(*this);
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log Completed";
}

BackupRestaurant::BackupRestaurant() {}

void BackupRestaurant::act(Restaurant &restaurant) {
  //Deleting previous backup and making a new backup via copy constructor.
    delete backup;
    backup=new Restaurant(restaurant);
    complete();
    restaurant.PushAction(*this);
}

std::string BackupRestaurant::toString() const {
    return "backup Completed";
}

RestoreResturant::RestoreResturant() {}

void RestoreResturant::act(Restaurant &restaurant) {
  //Restoring backup via copy assignment operator.
    if(backup==nullptr){
        this->error("No backup available");
    }
    else{
        restaurant=*backup;
        complete();
    }
    restaurant.PushAction(*this);
    if(this->getStatus()==ERROR)
        std::cout<< "Error: "<<this->getErrorMsg()<<std::endl;
}

std::string RestoreResturant::toString() const {
    std::string s="";
    if (getStatus() == COMPLETED)
        s=s+ "restore Completed";
    if (getStatus() == ERROR)
       s=s+ "restore Error: "+(this->getErrorMsg());
    return s;
}
OpenTable *OpenTable::clone() {
    return new OpenTable(*this);
}

Order *Order::clone() {
    return new Order(*this);
}

MoveCustomer *MoveCustomer::clone() {
    return new MoveCustomer(*this);}

Close *Close::clone() {
    return new Close(*this);}

CloseAll *CloseAll::clone() {
    return new CloseAll(*this);}

PrintMenu *PrintMenu::clone() {
    return new PrintMenu(*this);}

PrintTableStatus *PrintTableStatus::clone() {
    return new PrintTableStatus(*this);}

PrintActionsLog *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);}

BackupRestaurant *BackupRestaurant::clone() {
    return new BackupRestaurant(*this);}

RestoreResturant *RestoreResturant::clone() {
    return new RestoreResturant(*this);
}

Order::~Order() {}

MoveCustomer::~MoveCustomer() {

}

Close::~Close() {

}

CloseAll::~CloseAll() {

}

PrintMenu::~PrintMenu() {

}

PrintTableStatus::~PrintTableStatus() {

}

PrintActionsLog::~PrintActionsLog() {

}

BackupRestaurant::~BackupRestaurant() {

}

RestoreResturant::~RestoreResturant() {

}

BaseAction::~BaseAction() {}
























#include "../include/Table.h"
//#include "../include/Dish.h"
#include <iostream>

Table::Table(int t_capacity) : capacity(t_capacity) {
    open=false;
    bill=0;
}

int Table::getCapacity() const {
    return capacity;
}

std::vector<Customer *> &Table::getCustomers() {
    return customersList;
}

std::vector<OrderPair> &Table::getOrders() {
    return orderList;
}

bool Table::isOpen() {
    return open;
}

void Table::openTable() {
    open = true;
}
//close the table and clean all the customers,orders. set the bill to 0.
void Table::closeTable() {
    open = false;
    for (int i = 0;(unsigned) i < customersList.size(); i++) {
        delete customersList[i];
        customersList[i] = nullptr;
    }

    customersList.clear();
    orderList.clear();
    bill=0;
}

void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

Customer *Table::getCustomer(int id) {
    for (int i = 0;(unsigned) i < customersList.size(); i++)
        if (customersList[i]->getId() == id)
            return customersList[i];
    return nullptr;
}

void Table::removeCustomer(int id) {
    int cusIndex;
    for (int i = 0;(unsigned) i < customersList.size(); i++) {
	//look for the customer with the id.
        if (id == customersList[i]->getId())
            cusIndex = i;
    }
    customersList.erase(customersList.begin() + cusIndex);
}

int Table::getBill() {

    return bill;
}

void Table::order(const std::vector<Dish> &menu) {
    std::vector<int>  orders;
    for (int i = 0;(unsigned) i < this->getCustomers().size(); i++) {
	//vector of orders for every customer in the table.
        orders = this->getCustomers()[i]->order(menu);
        for (int j = 0;(unsigned) j < orders.size(); j++) {
	    //create orderpair for every order and push to orderlist.
            Dish dish = menu[orders[j]];
            OrderPair * pair=new OrderPair(this->getCustomers()[i]->getId(), dish);
            this->orderList.push_back(*pair);
	    //add the dish price to the bill.
            bill=bill+pair->second.getPrice();
            delete pair;
            std::cout << getCustomers()[i]->getName() + " " + "ordered" + " " + dish.getName()<< std::endl;
        }
    }

}

void Table::setBill(int toSet) {
    bill=toSet;
}


//------------------------------------rule of 5-------------------------------------------------------------------------------------//
//destructor
Table::~Table() {
    for(int i=0;(unsigned)i<customersList.size();i++) {
        if(customersList[i]->getStrategy()=="alc")
            customersList[i]->clearAlc();
        delete customersList[i];
            customersList[i] = nullptr;
        }

    orderList.clear();
    customersList.clear();
}
//copy constructor
Table::Table(const Table &other) :capacity(other.capacity) , open(other.open) ,bill(other.bill)   {
    customersList=std::vector<Customer*>();
    for(int i=0;(unsigned)i<other.customersList.size();i++)
        customersList.push_back(other.customersList[i]->clone());
    orderList=std::vector<OrderPair>();
    for(int i=0;(unsigned)i<other.orderList.size();i++)
        orderList.push_back(other.orderList[i]);
}
//copy assignment.
Table &Table::operator=(const Table& other) {
    if(this!=&other){
        capacity=other.capacity;
        open=other.open;
        bill=other.bill;
        orderList.clear();
        for(int i=0;(unsigned)i<customersList.size();i++) {
            delete customersList[i];
            customersList[i]= nullptr;
        }
        customersList.clear();

        customersList=std::vector<Customer*>();
        for(int i=0;(unsigned)i<other.customersList.size();i++)
            customersList.push_back(other.customersList[i]->clone());
        orderList=std::vector<OrderPair>();
        for(int i=0;(unsigned)i<other.orderList.size();i++)
            orderList.push_back(other.orderList[i]);
    }
    return (*this);
}
//move constructor.
Table::Table(Table &&other):Table(other.capacity)  {
    open=other.open;
    capacity=other.capacity;
    bill=other.bill;
    orderList.clear();
    for(int i=0;(unsigned)i<customersList.size();i++) {
        customersList[i]= nullptr;
    }
    customersList.clear();
    customersList=other.customersList;
    for(int i=0;(unsigned)i<other.customersList.size();i++) {
            other.customersList[i]= nullptr;
        }
    for(int i=0;(unsigned)i<other.orderList.size();i++) {
        orderList.push_back(other.orderList[i]);
    }
    other.customersList.clear();
    other.orderList.clear();

}
//move assignment
Table &Table::operator=(Table &&other) {
if(this!=&other){    
open=other.open;
    capacity=other.capacity;
    bill=other.bill;
    orderList.clear();
    for(int i=0;(unsigned)i<customersList.size();i++) {
        customersList[i]= nullptr;
    }
    customersList.clear();
    customersList=other.customersList;
    for(int i=0;(unsigned)i<other.customersList.size();i++) {
            other.customersList[i]= nullptr;
        }
    for(int i=0;(unsigned)i<other.orderList.size();i++) {
        orderList.push_back(other.orderList[i]);
    }
    other.customersList.clear();
    other.orderList.clear();
    }
   return (*this);

}

Table *Table::clone() {
    return new Table(*this);
}




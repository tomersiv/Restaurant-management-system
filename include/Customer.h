#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual std::string getStrategy ()  = 0;//A method for getting the customers' strategy.
    virtual Customer*  clone()=0;//Creating a virtual clone method for all customers.
    virtual void clearAlc()=0;//A method for clearing all previous orders made by alcoholic customer.
    virtual ~Customer();//A virtual destructor for all customers.
private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::string getStrategy();
    VegetarianCustomer * clone();
    void clearAlc();
private:
    std::string strategy;//We added a strategy field for each customer type.
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::string getStrategy();
    CheapCustomer * clone();
    void clearAlc();
private:
    bool isOrdered;
    std::string strategy;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::string getStrategy();
    SpicyCustomer * clone();
    void clearAlc();

private:
    int OrderCount;
    int menuBvgId;
    std::string strategy;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    bool ordered(int id);
    std::string getStrategy();
    AlchoholicCustomer * clone();
    void clearAlc();

private:
    int numOfAlc;
    int counter;
    std::vector<int> isOrdered;
    int mostExpId;
    std::string strategy;
};



#endif

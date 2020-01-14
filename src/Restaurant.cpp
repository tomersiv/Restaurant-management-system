#include "../include/Restaurant.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Restaurant::Restaurant() {}

const std::vector<BaseAction *> &Restaurant::getActionsLog() const {
    return actionsLog;
}

std::vector<Dish> &Restaurant::getMenu() {
    return menu;
}

Restaurant::Restaurant(const std::string &configFilePath) {
    int Tcapacity = 0;
    int numoftables = 0;
    std::string token;
    std::string line;
    std::string tables;
    int dishId = 0;
    std::string dishName;
    int dishPrice;
    DishType dishType;
    int numoflines=0;
    int lineCounter=0;
    int flag=0;
    std::ifstream configFile1(configFilePath);
    //calculate the number of lines in the file.
    if (configFile1.is_open()) {
        while (getline(configFile1, line))
            numoflines++;
    }
    bool menuTime = false;
    std::ifstream configFile(configFilePath);
    if (configFile.is_open()) {
        while (getline(configFile, line)) {
            lineCounter++;
            while ((line.empty() || line[0]=='#') && lineCounter <= numoflines) {
                getline(configFile, line);
                lineCounter++;
            }
            if (line[line.length() - 1] == '\r')
                line.erase(line.length() - 1);
	    //flag=0 means we are in the num of tables.
	    //flag=1 means we are in the tables description.
	    //flag=2 means we are in the menu.
            if (flag==0) {
	        //check if we are in empty line or comment line.
                while ((line.empty() || line[0]=='#') && lineCounter <= numoflines) {
                    getline(configFile, line);
                    lineCounter++;
                }
		//set the num of tables to the tables vector.
                numoftables = std::stoi(line);
                tables.resize(numoftables);
                flag=1;
                getline(configFile, line);
                lineCounter++;
            }
            if (flag==1) {
                while ((line.empty() || line[0]=='#') && lineCounter <= numoflines) {
                    getline(configFile, line);
                    lineCounter++;
                }
                std::istringstream is(line);
                while (getline(is, token, ',')) {
		//create table for every capacity in the file and enter the vector.
                    Tcapacity = std::stoi(token);
                    Table *t1 = new Table(Tcapacity);
                    this->tables.push_back(t1);
                }
                flag=2;
                getline(configFile, line);
                lineCounter++;
            }
            if (flag==2) {
                menuTime = true;
                while ((line.empty() || line[0]=='#') && lineCounter <= numoflines) {
                    getline(configFile, line);
                    lineCounter++;
                }
            }
            if (menuTime) {
                while (line.empty() && lineCounter <=numoflines) {
                    getline(configFile, line);
                    lineCounter++;
                }
		//check if we reach the end of file
                if (lineCounter > numoflines) {
                    break;
                }
		//enter every menu to the menu vector by name,price,type.
                std::istringstream is(line);
                getline(is, token, ',');
                dishName = token;
                getline(is, token, ',');
                if (token == "ALC")
                    dishType = ALC;
                if (token == "VEG")
                    dishType = VEG;
                if (token == "BVG")
                    dishType = BVG;
                if (token == "SPC")
                    dishType = SPC;
                getline(is, token, ',');
                dishPrice = std::stoi(token);
                Dish *d1 = new Dish(dishId, dishName, dishPrice, dishType);
                this->menu.push_back(*d1);
                delete d1;
                dishId++;
            }


        }
    }

    configFile.close();
}

void Restaurant::start() {
    std::cout << "Restaurant is now open!" << std::endl;
    std::string userAction = "";
    std::string firstWord = "";
    std::string customer = "";
    std::string name = "";
    std::string type = "";
    int tableId = 0;
    int index = 0;
    int cusIndex = 0;
    OpenTable* o1;
    getline(std::cin, userAction);
    while (userAction.compare("closeall") != 0) {
	//check the first word of the input command.
        for (int i = 0;(unsigned) i < userAction.length(); i++) {
            if (userAction[i] == ' ') {
                firstWord = (userAction.substr(0, i));
                index = i;
                break;
            }

            if((unsigned)i==userAction.length()-1)
                firstWord=userAction;
        }
        if (firstWord == "open") {
            std::vector<Customer*> customers;
            tableId = std::stoi(userAction.substr(index + 1, 1));
	    //check if to open table without customers.
            if(userAction.length()>6)
                userAction = userAction.substr(index + 3, userAction.length() - 6);
            if(userAction=="open "+std::to_string(tableId)) {
                o1 = new OpenTable(tableId, customers);
                o1->act(*this);
                customers.clear();
            }
            else {
		//add the customers to the customers vector.
                std::istringstream is(userAction);
                while (getline(is, customer, ' ')) {
                    std::istringstream is2(customer);
                    (getline(is2, name, ','));
                    (getline(is2, type, ','));
                    if (type == "veg") {
                        VegetarianCustomer *vegeterian = new VegetarianCustomer(name, cusIndex);
                        customers.push_back(vegeterian);
                    }
                    if (type == "alc") {
                        AlchoholicCustomer *alcoholic = new AlchoholicCustomer(name, cusIndex);
                        customers.push_back(alcoholic);
                    }
                    if (type == "chp") {
                        CheapCustomer *cheap = new CheapCustomer(name, cusIndex);
                        customers.push_back(cheap);
                    }
                    if (type == "spc") {
                        SpicyCustomer *spicy = new SpicyCustomer(name, cusIndex);
                        customers.push_back(spicy);
                    }
                    cusIndex++;
                }
		//start open table function.
                o1 = new OpenTable(tableId, customers);
                o1->act(*this);
                customers.clear();
            }
        }
        if (firstWord == "order") {
            Order * order;
            tableId = std::stoi(userAction.substr(index + 1, 1));
            order=new Order(tableId);
            order->act(*this);
        }

        if (firstWord == "move") {
            int sourceTable = std::stoi(userAction.substr(index + 1, 1));
            int destTable = std::stoi(userAction.substr(index + 3, 1));
            int customerId = std::stoi(userAction.substr(index + 5, 1));
            MoveCustomer* move=new MoveCustomer(sourceTable, destTable, customerId);
            move->act(*this);
        }
        if (firstWord == "close") {
            tableId = std::stoi(userAction.substr(index + 1, 1));
            Close * close=new Close(tableId);
            close->act(*this);
        }

        if (firstWord == "menu") {
            PrintMenu * printmenu = new PrintMenu;
            printmenu->act(*this);
        }
        if (firstWord == "status") {
            tableId = std::stoi(userAction.substr(index + 1, 1));
            PrintTableStatus * tablestatus=new PrintTableStatus(tableId);
            tablestatus->act(*this);
        }
        if (firstWord == "log") {
            PrintActionsLog * printactions=new PrintActionsLog();
            printactions->act(*this);
        }
        if (firstWord == "backup") {
            BackupRestaurant *backup=new BackupRestaurant;
            backup->act(*this);
        }
        if (firstWord == "restore") {
            RestoreResturant * restore=new RestoreResturant;
            restore->act(*this);
        }
        getline(std::cin, userAction);
        if (userAction == "closeall") {
            CloseAll * closeall=new CloseAll;
            closeall->act(*this);
        }
    }
}

    int Restaurant::getNumOfTables() const {
        return this->tables.size();
    }

    Table *Restaurant::getTable(int ind) {
       if ((unsigned)ind < 0 ||(unsigned) ind > this->tables.size() - 1)
            return nullptr;
        return tables[ind];
    }

    std::vector<Table *> Restaurant::getTables() {
        return this->tables;
    }

    void Restaurant::PushAction(BaseAction &action) {
        actionsLog.push_back(&action);
    }

//------------------------------------rule of 5-------------------------------------------------------------------------------------//
//destructor
Restaurant::~Restaurant() {
    this->menu.clear();
    for(int i=0;(unsigned)i<this->tables.size();i++){
        delete this->tables[i];
        this->tables[i]= nullptr;
    }

    for(int i=0;(unsigned)i<this->actionsLog.size();i++){
         delete(this->actionsLog[i]);
         this->actionsLog[i]= nullptr;
    }
    this->tables.clear();
    this->actionsLog.clear();


}
//copy constructor
Restaurant::Restaurant(const Restaurant &other): menu(other.menu) {
   for(int i=0;(unsigned)i<other.tables.size();i++){
       tables.push_back(other.tables[i]->clone());
   }
    for (int i=0;(unsigned)i<other.actionsLog.size();i++) {
        actionsLog.push_back(other.actionsLog[i]->clone());
    }

}
//copy assignment operator
Restaurant &Restaurant::operator=(const Restaurant &other) {
    if (this != &other) {
        menu.clear();
        menu=std::vector<Dish>();
        for(int i=0;(unsigned)i<other.menu.size();i++){
            Dish d=other.menu[i];
            this->menu.push_back(d);
        }
        for(int i=0;(unsigned)i<tables.size();i++) {
            delete tables[i];
            tables[i]= nullptr;
        }
        tables.clear();
        tables=std::vector<Table*>();
        for (int i = 0;(unsigned) i < other.tables.size(); i++)
            tables.push_back(other.tables[i]->clone());
        for(int i=0;(unsigned)i<actionsLog.size();i++) {
            delete actionsLog[i];
            actionsLog[i]=nullptr;
        }
        actionsLog.clear();
        actionsLog=std::vector<BaseAction*>();
        for (int i = 0;(unsigned) i < other.actionsLog.size(); i++)
            actionsLog.push_back(other.actionsLog[i]->clone());

    }
    return (*this);
}
//move constructor
Restaurant::Restaurant(Restaurant &&other) : menu(other.menu)  {
    tables=other.tables;
    actionsLog=other.actionsLog;
    tables.clear();
    for(int i=0;(unsigned)i<tables.size();i++) {
        tables[i] = nullptr;
    }
    for(int i=0;(unsigned)i<actionsLog.size();i++) {
        actionsLog[i]=nullptr;
    }
    tables=other.tables;
    actionsLog=other.actionsLog;
	for(int i=0;(unsigned)i<other.tables.size();i++) {
            other.tables[i]= nullptr;
        }
	for(int i=0;(unsigned)i<other.actionsLog.size();i++) {
            other.actionsLog[i]= nullptr;
        }
    other.tables.clear();
    other.menu.clear();
    other.actionsLog.clear();

}
//move assignment operator
Restaurant &Restaurant::operator=(Restaurant &&other) {
    if(this!=&other){
	menu.clear();
	for(int i=0;(unsigned)i<other.menu.size();i++){
	Dish d=other.menu[i];
	menu.push_back(d);
}
    tables=other.tables;
    actionsLog=other.actionsLog;
    tables.clear();
    for(int i=0;(unsigned)i<tables.size();i++) {
        tables[i] = nullptr;
    }
    for(int i=0;(unsigned)i<actionsLog.size();i++) {
        actionsLog[i]=nullptr;
    }
    tables=other.tables;
    actionsLog=other.actionsLog;
	for(int i=0;(unsigned)i<other.tables.size();i++) {
            other.tables[i]= nullptr;
        }
	for(int i=0;(unsigned)i<other.actionsLog.size();i++) {
            other.actionsLog[i]= nullptr;
        }
    other.tables.clear();
    other.menu.clear();
    other.actionsLog.clear();

    }
    return (*this);
}






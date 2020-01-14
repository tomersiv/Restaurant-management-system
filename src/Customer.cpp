#include "../include/Customer.h"

Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {}

int Customer::getId() const {
    return id;
}

std::string Customer::getName() const {
    return name;
}

VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(name, id) {this->strategy="veg";}

std::string VegetarianCustomer::getStrategy() {
    return this->strategy;
}

void VegetarianCustomer::clearAlc() {

}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> Veg;
    int smallestId;
    int foundInd;
    int expBvg = 0;
    int BvgId = 0;
    bool found = false;
    bool IsEmptyVEG = true;
    bool IsEmptyBVG = true;
    //check if menu is empty
    if(menu.size()==0)
        return Veg;
    //look for the cheapest veg dish.
    for (int i = 0;(((unsigned) i < menu.size()) & (!found)); i++)
        if (menu[i].getType() == VEG) {
            smallestId = menu[i].getId();
            found = true;
            foundInd = i;
            IsEmptyVEG = false;
        }
    //look for the smaller id if the price is the same.
    for (int i = foundInd;(unsigned) i < menu.size(); i++) {
        if (menu[i].getType() == VEG) {
            IsEmptyVEG = false;
            if (menu[i].getId() < smallestId)
                smallestId = menu[i].getId();
        }
    }
    //look for the most expensive bvg.
    for (int i = 0;(unsigned) i < menu.size(); i++)
        if (menu[i].getType() == BVG) {
            IsEmptyBVG = false;
            if (menu[i].getPrice() > expBvg) {
                expBvg = menu[i].getPrice();
                BvgId = menu[i].getId();
            }
	    //check for the smaller id if the price is the same.
            if (menu[i].getPrice() == expBvg)
                if (menu[i].getId() < BvgId)
                    BvgId = menu[i].getId();
        }
    if ((!IsEmptyVEG) & !IsEmptyBVG) {
        Veg.push_back(smallestId);
        Veg.push_back(BvgId);
    }
    return Veg;
}

void AlchoholicCustomer::clearAlc() {
    this->isOrdered.clear();
    this->isOrdered.shrink_to_fit();


}

CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) { isOrdered = false; this->strategy="chp"; }

std::string CheapCustomer::getStrategy() {
    return this-> strategy;
}

void CheapCustomer::clearAlc() {

}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> cheapestVec;
    //check if the menu is empty.
    if(menu.size()==0)
        return cheapestVec;
    //check if he is already ordered.
    if (this->isOrdered == true)
        return cheapestVec;
    this->isOrdered = true;
    int cheapest = menu[0].getPrice();
    int cheapestId = 0;
    //look for the cheapest dish.
    for (int i = 1;(unsigned) i < menu.size(); i++) {
        if (menu[i].getPrice() < cheapest) {
            cheapest = menu[i].getPrice();
            cheapestId = menu[i].getId();
        }
    }
    cheapestVec.push_back(cheapestId);
    return cheapestVec;

}

SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id), OrderCount(0), menuBvgId(-1) {this->strategy="spc";}

std::string SpicyCustomer::getStrategy() {
    return this-> strategy;
}

void SpicyCustomer::clearAlc() {

}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> Spc;
    int expSpc = 0;
    int foundInd;
    bool IsEmptySpc = true;
    bool IsEmptyNonAlc = true;
    bool found = false;
    int SpcId = 0;
    int ChpNonAlcPrice = 0;
    int ChpNonAlcId = 0;
    //check if the menu is empty
    if(menu.size()==0)
        return Spc;
    //order 1. look for the most expensive spc.
    if (OrderCount == 0) {
        for (int i = 0;(unsigned) i < menu.size(); i++) {
            if (menu[i].getType() == SPC) {
                IsEmptySpc = false;
                if (menu[i].getPrice() > expSpc) {
                    expSpc = menu[i].getPrice();
                    SpcId = menu[i].getId();
                }
            }
        }
        if (!IsEmptySpc) {
            Spc.push_back(SpcId);
            OrderCount++;
            return Spc;
        } else
            return Spc;
    }
    //order 2.look for the cheapest non alcoholic.
    if (OrderCount == 1) {
        for (int i = 0;(((unsigned) i < menu.size()) & (!found)); i++) {
            if (menu[i].getType() == BVG) {
                ChpNonAlcPrice = menu[i].getPrice();
                ChpNonAlcId = menu[i].getId();
                IsEmptyNonAlc = false;
                foundInd = i;
                this->menuBvgId = ChpNonAlcId;
                found = true;
            }
        }
        for (int i = foundInd;(unsigned) i < menu.size(); i++) {
            if (menu[i].getType() == BVG) {
                IsEmptyNonAlc = false;
                if (menu[i].getPrice() < ChpNonAlcPrice) {
                    ChpNonAlcPrice = menu[i].getPrice();
                    ChpNonAlcId = menu[i].getId();
                    this->menuBvgId = ChpNonAlcId;
                }
		//if the price is the same look for the lower id.
                if (menu[i].getPrice() == ChpNonAlcPrice) {
                    if (menu[i].getId() < ChpNonAlcId) {
                        ChpNonAlcId = menu[i].getId();
                        this->menuBvgId = ChpNonAlcId;
                    }
                }
            }
        }
        if (!IsEmptyNonAlc)
            Spc.push_back(ChpNonAlcId);
        return Spc;
    }
    //order 3 and more. order the same bvg.
    if (this->OrderCount > 1) {
        Spc.push_back(this->menuBvgId);
        return Spc;
    }
    return Spc;
}

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(name, id) {
    this->strategy="alc";
    this->counter=0;
    this->numOfAlc=0;
    this->mostExpId=-1;
}

std::string AlchoholicCustomer::getStrategy() {
    return this->strategy;
}
//function that chack if he is ordered.
bool AlchoholicCustomer::ordered(int id) {
    bool wasOrder = false;
    for (int i = 0;(((unsigned) i < isOrdered.size()) & (!wasOrder)); i++) {
        if (isOrdered[i] == id)
            wasOrder = true;
    }
    return wasOrder;

}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> Alc;
    bool found = false;
    int indexAlc = -1;
    int Alcprice = 0;
    int Alcid = 0;
    //check if menu is empty.
    if(menu.size()==0)
        return Alc;
    //count the nums of alcoholic dish in the menu.
    if (this->counter == 0) {
        for (int i = 0;(unsigned) i < menu.size(); i++) {
            if (menu[i].getType() == ALC) {
                this->numOfAlc++;
            }
        }
    }
    //check if there is no alcoholic dished in the menu
    if (numOfAlc == 0)
        return Alc;
    //orders before order all the alcoholic dishes.
    if (this->counter >= 0 && this->counter < this->numOfAlc) {
        for (int i = 0;(((unsigned) i < menu.size()) & (!found)); i++) {
            if (menu[i].getType() == ALC) {
                if (!ordered(menu[i].getId())) {
                    found = true;
                    indexAlc = i;
                    Alcprice = menu[i].getPrice();
                    Alcid = menu[i].getId();
                }
            }
        }
        for (int i = indexAlc;(unsigned) i < menu.size(); i++) {
            if (menu[i].getType() == ALC) {
                if (!ordered(menu[i].getId())) {
                    if (menu[i].getPrice() < Alcprice) {
                        Alcprice = menu[i].getPrice();
                        Alcid = menu[i].getId();
                    }
                    if (menu[i].getPrice() == Alcprice)
                        if (menu[i].getId() < Alcid)
                            Alcid = menu[i].getPrice();
                }
            }
        }
    }
    //last alcoholic dish order.
    if (this->counter == this->numOfAlc - 1) {
        for (int i = 0;(unsigned) i < menu.size(); i++) {
            if (menu[i].getType() == ALC) {
                if (!ordered(menu[i].getId())) {
                    this->mostExpId = menu[i].getId();
                    Alcid = menu[i].getId();
                    Alcprice = menu[i].getPrice();
                }
            }
        }
    }
    //orders after order all the alcoholic dishes.
    if (this->counter > this->numOfAlc - 1)
        return Alc;


    Alc.push_back(Alcid);
    this->isOrdered.push_back(Alcid);
    this->counter++;
    return Alc;

}


std::string VegetarianCustomer::toString() const {
    return "name "+ this->getName()+" id "+std::to_string(this->getId());
}

std::string CheapCustomer::toString() const {
    return "name "+ this->getName()+" id "+std::to_string(this->getId());
}

std::string SpicyCustomer::toString() const {
    return "name "+ this->getName()+" id "+std::to_string(this->getId());
}

std::string AlchoholicCustomer::toString() const {
    return "name "+ this->getName()+" id "+std::to_string(this->getId());
}

VegetarianCustomer *VegetarianCustomer::clone() {
    return new VegetarianCustomer(*this);
}

CheapCustomer *CheapCustomer::clone() {
    return new CheapCustomer(*this);
}

SpicyCustomer *SpicyCustomer::clone() {
    return new SpicyCustomer(*this);
}

AlchoholicCustomer *AlchoholicCustomer::clone() {
    return new AlchoholicCustomer(*this);
}
//empty distructor.
Customer::~Customer() {
}


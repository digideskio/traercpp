#pragma once

#include "LinkedList.h"
#include "ListItem.h"

class Force : public ListItem {
public:
 Force(){}
 ~Force(){}
 virtual void turnOn() {}
 virtual void turnOff() {}
 virtual bool isOn() { return true; }
 virtual bool isOff() { return false; }
 virtual void apply() {}
};

class Forces : public LinkedList {
};
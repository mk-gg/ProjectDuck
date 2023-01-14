#pragma once
#include <boost/python.hpp>
#include <iostream>

#include "GameObject.h"


void greet()
{
    std::cout << "Added static data";
}


BOOST_PYTHON_MODULE(Duck)
{
    using namespace boost::python;
	class_<GameObject>("Obj", "In-game Object.")
		.def_readonly("name", &GameObject::name)
		.def_readonly("index", &GameObject::index)
		.def_readonly("net_id", &GameObject::networkId)
		.def_readonly("team", &GameObject::team)
		.def_readonly("pos", &GameObject::pos)
		.def_readonly("visible", &GameObject::isVisible)
		.def_readonly("last_seen", &GameObject::lastSeen);
           
    def("greet", greet);

}
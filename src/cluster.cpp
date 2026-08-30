#include "cluster.h"

Cluster::Cluster(){}
Cluster::~Cluster(){
    this->active = nullptr; 
    for (auto i: this->modules) {
        delete i;
    }
};

void Cluster::setConfig(Config config){

}
#include "Hexagon.h"

std::ostream& operator<<(std::ostream &os , const Hexagon& hex){
    os << "Hexagon(" << hex.getQ() << ", " << hex.getR() << ")";
    return os;
}
#include <sstream>

std::string Hexagon::toJson() const {
    std::stringstream jsonData;
    jsonData << "["<<q<<","<<r<<"]";
    return jsonData.str();
}

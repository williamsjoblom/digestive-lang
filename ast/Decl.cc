#include "Decl.hh"


Decl::Decl(std::string identifier) {
    this->identifier = identifier;
}


bool Decl::equals(const Node &other) const {
    const Decl* o = dynamic_cast<const Decl*>(&other);
    if (o == nullptr) return false;

    return o->identifier == identifier;
}

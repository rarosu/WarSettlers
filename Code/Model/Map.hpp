#ifndef MODEL_MAP_HPP
#define MODEL_MAP_HPP

#include <map>
#include "Entity.hpp"

namespace Model
{
    class Map
    {
    public:
        Map();

        void AddEntity(const Entity& entity);
        std::map<int, Entity>*& GetEntities();
    private:
        std::map<int, Entity> m_entities;
        unsigned int m_gridWidth;
        unsigned int m_gridHeight;
    };
}

#endif
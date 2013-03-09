#ifndef MODEL_ENTITY_HPP
#define MODEL_ENTITY_HPP

#include <Framework\D3DInclude.hpp>

namespace Model
{
    struct Entity
    {
        Entity(const D3DXVECTOR3& position);

        void SetDestination(const D3DXVECTOR3& destination);
        void Update(float dt);

        D3DXVECTOR3 m_gridPosition;
        D3DXVECTOR3 m_destination;
        float m_speed;
        float m_acceleration;
    };
}

#endif
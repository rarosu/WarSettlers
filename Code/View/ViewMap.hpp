#ifndef VIEW_MAP_HPP
#define VIEW_MAP_HPP

#include "ViewEntity.hpp"
#include <vector>

namespace View
{
	class ViewMap
	{
	public:
		ViewMap();
		~ViewMap();
	private: 
		float m_squareSize; 
		// width and height in number of squares. 
		int m_width; 
		int m_height; 
		std::vector<ViewEntity> m_entities; 
	};
}
#endif 
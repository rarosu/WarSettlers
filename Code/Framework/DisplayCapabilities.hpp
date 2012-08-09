#ifndef FRAMEWORK_DISPLAY_CAPABILITIES
#define FRAMEWORK_DISPLAY_CAPABILITIES

#include <vector>
#include "D3DInclude.hpp"

namespace Framework
{
	/**
		Describes one supported display mode for a given monitor & adapter.
	*/
	struct DisplayMode
	{
		DXGI_MODE_DESC m_description;


		DisplayMode();

		bool operator==(const DisplayMode& rhs) const;
		bool operator!=(const DisplayMode& rhs) const;
		bool operator<(const DisplayMode& rhs) const;
		bool operator<=(const DisplayMode& rhs) const;
		bool operator>(const DisplayMode& rhs) const;
		bool operator>=(const DisplayMode& rhs) const;
	};

	/**
		This class enumerates the supported display modes and other
		interesting graphics capabilities.
	*/
	class DisplayCapabilities
	{
	public:
		DisplayCapabilities(bool sortDisplayModes);

		DXGI_ADAPTER_DESC m_adapterDescription;
		DXGI_OUTPUT_DESC m_outputDescription;
		std::vector<DisplayMode> m_displayModes;
	};
}

#endif
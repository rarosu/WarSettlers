#include "DisplayCapabilities.hpp"

#include <algorithm>
#include <cstdlib>
#include "COMResource.hpp"
#include <Libs\r2tk\r2-exception.hpp>


namespace Framework
{
	DisplayMode::DisplayMode()
	{
		m_description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_description.Width = 800;
		m_description.Height = 600;
		m_description.RefreshRate.Numerator = 60;
		m_description.RefreshRate.Denominator = 1;
		m_description.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		m_description.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	}


	bool DisplayMode::operator==(const DisplayMode& rhs) const
	{
		return memcmp(&m_description, &rhs.m_description, sizeof(DXGI_MODE_DESC)) == 0;
	}

	bool DisplayMode::operator!=(const DisplayMode& rhs) const
	{
		return !((*this) == rhs);
	}

	bool DisplayMode::operator<(const DisplayMode& rhs) const
	{
		return m_description.Width < rhs.m_description.Width;
	}

	bool DisplayMode::operator<=(const DisplayMode& rhs) const
	{
		return ((*this) < rhs || (*this) == rhs);
	}

	bool DisplayMode::operator>(const DisplayMode& rhs) const
	{
		return m_description.Width > rhs.m_description.Width;
		//return !((*this) <= rhs);
	}

	bool DisplayMode::operator>=(const DisplayMode& rhs) const
	{
		return !((*this) < rhs);
	}



	bool SortModesDescending(const DisplayMode& lhs, const DisplayMode& rhs)
	{
		return lhs > rhs;
	}

	DisplayCapabilities::DisplayCapabilities(bool sortDisplayModes)
	{
		HRESULT result = S_OK;

		// Create DXGI factory
		COMResource<IDXGIFactory> factory;
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (LPVOID*)&factory.Resource());
		if (FAILED(result))
			throw r2ExceptionRuntimeM("Failed to create DXGI factory");

		// Find the first DXGI adapter (primary video card)
		COMResource<IDXGIAdapter> adapter;
		result = factory->EnumAdapters(0, &adapter.Resource());
		if (FAILED(result))
			throw r2ExceptionRuntimeM("Failed to access primary DXGI adapter"); 
		result = adapter->GetDesc(&m_adapterDescription);
		if (FAILED(result))
			throw r2ExceptionRuntimeM("Failed to access primary DXGI adapter description");

		// Find the first DXGI output for the adapter (primary monitor connected to the primary video card)
		COMResource<IDXGIOutput> output;
		result = adapter->EnumOutputs(0, &output.Resource());
		if (FAILED(result))
			throw r2ExceptionRuntimeM("Failed to access primary DXGI output from the primary DXGI adapter");
		result = output->GetDesc(&m_outputDescription);
		if (FAILED(result))
			throw r2ExceptionRuntimeM("Failed to access primary DXGI output description");


		// Find the number of display modes available for the given output
		unsigned int displayModeCount;
		result = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &displayModeCount, NULL);
		if (FAILED(result))
			throw r2ExceptionRuntimeM("Failed to access number of display modes");

		// List all display modes available from the DXGI output
		std::vector<DXGI_MODE_DESC> displayModes(displayModeCount);
		result = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &displayModeCount, &displayModes[0]);
		if (FAILED(result))
			throw r2ExceptionRuntimeM("Failed to list display modes");

		// Store the display modes
		m_displayModes.resize(displayModeCount);
		for (size_t i = 0; i < displayModeCount; ++i)
		{
			m_displayModes[i].m_description = displayModes[i];
		}

		// Sort the list if requested
		if (sortDisplayModes)
		{
			std::sort(m_displayModes.begin(), m_displayModes.end(), SortModesDescending);
		}
	}
}
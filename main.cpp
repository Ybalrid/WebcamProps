#include <algorithm>
#include <objbase.h>

#include <vector>
#include <iostream>

#include "libdshowcapture/dshowcapture.hpp"

using device_vec = std::vector<DShow::VideoDevice>;

size_t lazy_find(const device_vec& list, const std::string& name)
{
	for (size_t i = 0; i < list.size(); ++i)
	{
		const std::wstring unicode_dev_name = list[i].name;
		//this is very incorrect, hope your webcam name aint in UNICODE!! >.<
		std::string dev_name{ unicode_dev_name.begin(), unicode_dev_name.end() };
		std::ranges::transform(dev_name, dev_name.begin(), [](const char c) -> char
		{
			return std::tolower((int)c);  // NOLINT(clang-diagnostic-implicit-int-conversion)
		});

		if (strstr(dev_name.c_str(), name.c_str()) != nullptr)
			return i;
	}

	throw std::runtime_error("cannot find your dang webcam called " + name);
}

//we use Win32 subsystem, but still want the UNIX C entry point please
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
int main(int argc, char** argv)
{
	//make sure args[0] will contain a string
	std::vector<std::string> args(argc - 1);
	for (int i = 1; i < argc; ++i) args[i - 1] = argv[i];

	if (args.empty())
	{
		MessageBoxA(NULL, "Please provide webcam hint string", "cmdline error", MB_ICONERROR);
		return -1;
	}

	CoInitialize(nullptr);
	device_vec devices;
	DShow::Device::EnumVideoDevices(devices);

	try
	{
		const auto index = lazy_find(devices, args[0]);
		const auto& webcam = devices[index];

		DShow::VideoConfig config{};
		config.name = webcam.name;
		config.path = webcam.path;

		//create a dummy libdshowdevice, we ain't gonna capture but it needs a name and path
		DShow::Device webcam_device{};
		webcam_device.ResetGraph();
		webcam_device.SetVideoConfig(&config);

		//let them call the one stupid API we wanted to call in the first place.
		webcam_device.OpenDialog(nullptr, DShow::DialogType::ConfigVideo);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Whoops!", MB_ICONERROR);
	}

	CoUninitialize();
	return 0;
}

//---------------------------------------------------------------------------
//
// SCSI Target Emulator RaSCSI Reloaded
// for Raspberry Pi
//
// Copyright (C) 2022 Uwe Seimet
//
// Unit tests based on GoogleTest and GoogleMock
//
//---------------------------------------------------------------------------

#include <gtest/gtest.h>

#include "rascsi_version.h"
#include "../devices/device.h"
#include "../devices/device_factory.h"

using namespace rascsi_interface;

namespace DeviceFactoryTest
{

DeviceFactory& device_factory = DeviceFactory::instance();

TEST(DeviceFactoryTest, GetTypeForFile)
{
	EXPECT_EQ(device_factory.GetTypeForFile("test.hdf"), SAHD);
	EXPECT_EQ(device_factory.GetTypeForFile("test.hds"), SCHD);
	EXPECT_EQ(device_factory.GetTypeForFile("test.HDS"), SCHD);
	EXPECT_EQ(device_factory.GetTypeForFile("test.hda"), SCHD);
	EXPECT_EQ(device_factory.GetTypeForFile("test.hdn"), SCHD);
	EXPECT_EQ(device_factory.GetTypeForFile("test.hdi"), SCHD);
	EXPECT_EQ(device_factory.GetTypeForFile("test.nhd"), SCHD);
	EXPECT_EQ(device_factory.GetTypeForFile("test.hdr"), SCRM);
	EXPECT_EQ(device_factory.GetTypeForFile("test.mos"), SCMO);
	EXPECT_EQ(device_factory.GetTypeForFile("test.iso"), SCCD);
	EXPECT_EQ(device_factory.GetTypeForFile("test.suffix.iso"), SCCD);
	EXPECT_EQ(device_factory.GetTypeForFile("bridge"), SCBR);
	EXPECT_EQ(device_factory.GetTypeForFile("daynaport"), SCDP);
	EXPECT_EQ(device_factory.GetTypeForFile("printer"), SCLP);
	EXPECT_EQ(device_factory.GetTypeForFile("services"), SCHS);
	EXPECT_EQ(device_factory.GetTypeForFile("unknown"), UNDEFINED);
	EXPECT_EQ(device_factory.GetTypeForFile("test.iso.suffix"), UNDEFINED);
}

TEST(DeviceFactoryTest, GetSectorSizes)
{
	unordered_set<uint32_t> sector_sizes;

	sector_sizes = device_factory.GetSectorSizes("SCHD");
	EXPECT_EQ(4, sector_sizes.size());
	sector_sizes = device_factory.GetSectorSizes(SCHD);
	EXPECT_EQ(4, sector_sizes.size());

	EXPECT_TRUE(sector_sizes.find(512) != sector_sizes.end());
	EXPECT_TRUE(sector_sizes.find(1024) != sector_sizes.end());
	EXPECT_TRUE(sector_sizes.find(2048) != sector_sizes.end());
	EXPECT_TRUE(sector_sizes.find(4096) != sector_sizes.end());

	sector_sizes = device_factory.GetSectorSizes("SCRM");
	EXPECT_EQ(4, sector_sizes.size());
	sector_sizes = device_factory.GetSectorSizes(SCRM);
	EXPECT_EQ(4, sector_sizes.size());

	EXPECT_TRUE(sector_sizes.find(512) != sector_sizes.end());
	EXPECT_TRUE(sector_sizes.find(1024) != sector_sizes.end());
	EXPECT_TRUE(sector_sizes.find(2048) != sector_sizes.end());
	EXPECT_TRUE(sector_sizes.find(4096) != sector_sizes.end());

	sector_sizes = device_factory.GetSectorSizes("SCMO");
	EXPECT_EQ(4, sector_sizes.size());
	sector_sizes = device_factory.GetSectorSizes(SCMO);
	EXPECT_EQ(4, sector_sizes.size());

	EXPECT_TRUE(sector_sizes.find(512) != sector_sizes.end());
	EXPECT_TRUE(sector_sizes.find(1024) != sector_sizes.end());
	EXPECT_TRUE(sector_sizes.find(2048) != sector_sizes.end());
	EXPECT_TRUE(sector_sizes.find(4096) != sector_sizes.end());

	sector_sizes = device_factory.GetSectorSizes("SCCD");
	EXPECT_EQ(2, sector_sizes.size());
	sector_sizes = device_factory.GetSectorSizes(SCCD);
	EXPECT_EQ(2, sector_sizes.size());

	EXPECT_TRUE(sector_sizes.find(512) != sector_sizes.end());
	EXPECT_TRUE(sector_sizes.find(2048) != sector_sizes.end());
}

TEST(DeviceFactoryTest, UnknownDeviceType)
{
	Device *device = device_factory.CreateDevice(UNDEFINED, "test");
	EXPECT_EQ(nullptr, device);
}

TEST(DeviceFactoryTest, SCHD_Device_Defaults)
{
	Device *device = device_factory.CreateDevice(UNDEFINED, "test.hda");
	EXPECT_NE(nullptr, device);
	EXPECT_TRUE(device->SupportsFile());
	EXPECT_FALSE(device->SupportsParams());
	EXPECT_TRUE(device->IsProtectable());
	EXPECT_FALSE(device->IsProtected());
	EXPECT_FALSE(device->IsReadOnly());
	EXPECT_FALSE(device->IsRemovable());
	EXPECT_FALSE(device->IsRemoved());
	EXPECT_FALSE(device->IsLockable());
	EXPECT_FALSE(device->IsLocked());
	EXPECT_TRUE(device->IsStoppable());
	EXPECT_FALSE(device->IsStopped());

	EXPECT_EQ("QUANTUM", device->GetVendor()) << "Invalid default vendor for Apple drive";
	EXPECT_EQ("FIREBALL", device->GetProduct()) << "Invalid default vendor for Apple drive";
	EXPECT_EQ(string(rascsi_get_version_string()).substr(0, 2) + string(rascsi_get_version_string()).substr(3, 2),
			device->GetRevision());

	EXPECT_EQ(32, device->GetSupportedLuns());
}

TEST(DeviceFactoryTest, SCRM_Device_Defaults)
{
	Device *device = device_factory.CreateDevice(UNDEFINED, "test.hdr");
	EXPECT_NE(nullptr, device);
	EXPECT_TRUE(device->SupportsFile());
	EXPECT_FALSE(device->SupportsParams());
	EXPECT_TRUE(device->IsProtectable());
	EXPECT_FALSE(device->IsProtected());
	EXPECT_FALSE(device->IsReadOnly());
	EXPECT_TRUE(device->IsRemovable());
	EXPECT_FALSE(device->IsRemoved());
	EXPECT_TRUE(device->IsLockable());
	EXPECT_FALSE(device->IsLocked());
	EXPECT_TRUE(device->IsStoppable());
	EXPECT_FALSE(device->IsStopped());

	EXPECT_EQ("RaSCSI", device->GetVendor());
	EXPECT_EQ("SCSI HD (REM.)", device->GetProduct());
	EXPECT_EQ(string(rascsi_get_version_string()).substr(0, 2) + string(rascsi_get_version_string()).substr(3, 2),
			device->GetRevision());

	EXPECT_EQ(32, device->GetSupportedLuns());
}

TEST(DeviceFactoryTest, SCMO_Device_Defaults)
{
	Device *device = device_factory.CreateDevice(UNDEFINED, "test.mos");
	EXPECT_NE(nullptr, device);
	EXPECT_TRUE(device->SupportsFile());
	EXPECT_FALSE(device->SupportsParams());
	EXPECT_TRUE(device->IsProtectable());
	EXPECT_FALSE(device->IsProtected());
	EXPECT_FALSE(device->IsReadOnly());
	EXPECT_TRUE(device->IsRemovable());
	EXPECT_FALSE(device->IsRemoved());
	EXPECT_TRUE(device->IsLockable());
	EXPECT_FALSE(device->IsLocked());
	EXPECT_TRUE(device->IsStoppable());
	EXPECT_FALSE(device->IsStopped());

	EXPECT_EQ("RaSCSI", device->GetVendor());
	EXPECT_EQ("SCSI MO", device->GetProduct());
	EXPECT_EQ(string(rascsi_get_version_string()).substr(0, 2) + string(rascsi_get_version_string()).substr(3, 2),
			device->GetRevision());

	EXPECT_EQ(32, device->GetSupportedLuns());
}

TEST(DeviceFactoryTest, SCCD_Device_Defaults)
{
	Device *device = device_factory.CreateDevice(UNDEFINED, "test.iso");
	EXPECT_NE(nullptr, device);
	EXPECT_TRUE(device->SupportsFile());
	EXPECT_FALSE(device->SupportsParams());
	EXPECT_FALSE(device->IsProtectable());
	EXPECT_FALSE(device->IsProtected());
	EXPECT_TRUE(device->IsReadOnly());
	EXPECT_TRUE(device->IsRemovable());
	EXPECT_FALSE(device->IsRemoved());
	EXPECT_TRUE(device->IsLockable());
	EXPECT_FALSE(device->IsLocked());
	EXPECT_TRUE(device->IsStoppable());
	EXPECT_FALSE(device->IsStopped());

	EXPECT_EQ("RaSCSI", device->GetVendor());
	EXPECT_EQ("SCSI CD-ROM", device->GetProduct());
	EXPECT_EQ(string(rascsi_get_version_string()).substr(0, 2) + string(rascsi_get_version_string()).substr(3, 2),
			device->GetRevision());

	EXPECT_EQ(32, device->GetSupportedLuns());
}

TEST(DeviceFactoryTest, SCBR_Device_Defaults)
{
	Device *device = device_factory.CreateDevice(UNDEFINED, "bridge");
	EXPECT_NE(nullptr, device);
	EXPECT_FALSE(device->SupportsFile());
	EXPECT_TRUE(device->SupportsParams());
	EXPECT_FALSE(device->IsProtectable());
	EXPECT_FALSE(device->IsProtected());
	EXPECT_FALSE(device->IsReadOnly());
	EXPECT_FALSE(device->IsRemovable());
	EXPECT_FALSE(device->IsRemoved());
	EXPECT_FALSE(device->IsLockable());
	EXPECT_FALSE(device->IsLocked());
	EXPECT_FALSE(device->IsStoppable());
	EXPECT_FALSE(device->IsStopped());

	EXPECT_EQ("RaSCSI", device->GetVendor());
	EXPECT_EQ("SCSI HOST BRIDGE", device->GetProduct());
	EXPECT_EQ(string(rascsi_get_version_string()).substr(0, 2) + string(rascsi_get_version_string()).substr(3, 2),
			device->GetRevision());

	EXPECT_EQ(32, device->GetSupportedLuns());
}

TEST(DeviceFactoryTest, SCDP_Device_Defaults)
{
	Device *device = device_factory.CreateDevice(UNDEFINED, "daynaport");
	EXPECT_NE(nullptr, device);
	EXPECT_FALSE(device->SupportsFile());
	EXPECT_TRUE(device->SupportsParams());
	EXPECT_FALSE(device->IsProtectable());
	EXPECT_FALSE(device->IsProtected());
	EXPECT_FALSE(device->IsReadOnly());
	EXPECT_FALSE(device->IsRemovable());
	EXPECT_FALSE(device->IsRemoved());
	EXPECT_FALSE(device->IsLockable());
	EXPECT_FALSE(device->IsLocked());
	EXPECT_FALSE(device->IsStoppable());
	EXPECT_FALSE(device->IsStopped());

	EXPECT_EQ("Dayna", device->GetVendor());
	EXPECT_EQ("SCSI/Link", device->GetProduct());
	EXPECT_EQ("1.4a", device->GetRevision());

	EXPECT_EQ(32, device->GetSupportedLuns());
}

TEST(DeviceFactoryTest, SCHS_Device_Defaults)
{
	Device *device = device_factory.CreateDevice(UNDEFINED, "services");
	EXPECT_NE(nullptr, device);
	EXPECT_FALSE(device->SupportsFile());
	EXPECT_FALSE(device->SupportsParams());
	EXPECT_FALSE(device->IsProtectable());
	EXPECT_FALSE(device->IsProtected());
	EXPECT_FALSE(device->IsReadOnly());
	EXPECT_FALSE(device->IsRemovable());
	EXPECT_FALSE(device->IsRemoved());
	EXPECT_FALSE(device->IsLockable());
	EXPECT_FALSE(device->IsLocked());
	EXPECT_FALSE(device->IsStoppable());
	EXPECT_FALSE(device->IsStopped());

	EXPECT_EQ("RaSCSI", device->GetVendor());
	EXPECT_EQ("Host Services", device->GetProduct());
	EXPECT_EQ(string(rascsi_get_version_string()).substr(0, 2) + string(rascsi_get_version_string()).substr(3, 2),
			device->GetRevision());

	EXPECT_EQ(32, device->GetSupportedLuns());
}

TEST(DeviceFactoryTest, SCLP_Device_Defaults)
{
	Device *device = device_factory.CreateDevice(UNDEFINED, "printer");
	EXPECT_NE(nullptr, device);
	EXPECT_FALSE(device->SupportsFile());
	EXPECT_TRUE(device->SupportsParams());
	EXPECT_FALSE(device->IsProtectable());
	EXPECT_FALSE(device->IsProtected());
	EXPECT_FALSE(device->IsReadOnly());
	EXPECT_FALSE(device->IsRemovable());
	EXPECT_FALSE(device->IsRemoved());
	EXPECT_FALSE(device->IsLockable());
	EXPECT_FALSE(device->IsLocked());
	EXPECT_FALSE(device->IsStoppable());
	EXPECT_FALSE(device->IsStopped());

	EXPECT_EQ("RaSCSI", device->GetVendor());
	EXPECT_EQ("SCSI PRINTER", device->GetProduct());
	EXPECT_EQ(string(rascsi_get_version_string()).substr(0, 2) + string(rascsi_get_version_string()).substr(3, 2),
			device->GetRevision());

	EXPECT_EQ(32, device->GetSupportedLuns());
}

}
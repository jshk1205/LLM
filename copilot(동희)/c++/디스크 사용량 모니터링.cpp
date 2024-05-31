//디스크 사용량 모니터링이라는 프로그램을 만들겁니다.
//파일의 크기를 알려주는 기능
//어느 타입의 파일이 많은지 알려주는 기능


#include <iostream>
#include <string>
#include <iomanip>
#include <windows.h>
#include <tchar.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <vector>

using namespace std;

class DiskMonitor
{
public:
    DiskMonitor()
    {
        this->diskName = L"";
        this->diskSize = 0;
        this->diskType = L"";
        this->diskFreeSpace = 0;
        this->diskTotalSpace = 0;
    }
    ~DiskMonitor()
    {
    }
    void getDiskSize()
    {
        ULARGE_INTEGER freeBytesAvailable;
        ULARGE_INTEGER totalNumberOfBytes;
        ULARGE_INTEGER totalNumberOfFreeBytes;
        GetDiskFreeSpaceEx(this->diskName.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes);
        this->diskSize = totalNumberOfBytes.QuadPart;
        this->diskFreeSpace = freeBytesAvailable.QuadPart;
        this->diskTotalSpace = totalNumberOfFreeBytes.QuadPart;
    }
    void getDiskType()
    {
        TCHAR volumeNameBuffer[MAX_PATH + 1];
        TCHAR fileSystemNameBuffer[MAX_PATH + 1];
        DWORD serialNumber;
        DWORD maxComponentLength;
        DWORD fileSystemFlags;
        GetVolumeInformation(this->diskName.c_str(), volumeNameBuffer, MAX_PATH + 1, &serialNumber, &maxComponentLength, &fileSystemFlags, fileSystemNameBuffer, MAX_PATH + 1);
        this->diskType = fileSystemNameBuffer;
    }
    void getDiskUsage()
    {
        PDH_STATUS status;
        PDH_HQUERY query;
        PDH_HCOUNTER counter;
        status = PdhOpenQuery(NULL, 0, &query);
        if (status != ERROR_SUCCESS)
        {
            return;
        }
        status = PdhAddCounter(query, L"\\PhysicalDisk(_Total)\\% Disk Time", 0, &counter);
        if (status != ERROR_SUCCESS)
        {
            return;
        }
        status = PdhCollectQueryData(query);
        if (status != ERROR_SUCCESS)
        {
            return;
        }
        PDH_FMT_COUNTERVALUE value;
        status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &value);
        if (status != ERROR_SUCCESS)
        {
            return;
        }
        this->diskUsage = value.doubleValue;
        PdhCloseQuery(query);
    }
    void getDiskUsageHistory()
    {
        PDH_STATUS status;
        PDH_HQUERY query;
        PDH_HCOUNTER counter;

        // 쿼리를 엽니다.
        status = PdhOpenQuery(NULL, NULL, &query);
        if (status != ERROR_SUCCESS)
        {
            return;
        }

        status = PdhAddCounter(query, L"\\PhysicalDisk(_Total)\\% Disk Time", 0, &counter);
        if (status != ERROR_SUCCESS)
        {
            return;
        }

        status = PdhCollectQueryData(query);
        if (status != ERROR_SUCCESS)
        {
            return;
        }

        PDH_FMT_COUNTERVALUE value;
        status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &value);
        if (status != ERROR_SUCCESS)
        {
            return;
        }

        this->diskUsageHistory.push_back(value.doubleValue);
        PdhCloseQuery(query);
    }
    void setDiskName(wstring diskName)
    {
        this->diskName = diskName;
    }
    void printDiskSize()
    {
        wcout << L"Disk Size: " << this->diskSize << L" bytes" << endl;
    }
    void printDiskType()
    {
        wcout << L"Disk Type: " << this->diskType << endl;
    }
    void printDiskUsage()
    {
        wcout << L"Disk Usage: " << this->diskUsage << L"%" << endl;
    }


private:
    wstring diskName;
    ULONGLONG diskSize;
    wstring diskType;
    ULONGLONG diskFreeSpace;
    ULONGLONG diskTotalSpace;
    double diskUsage;
    vector<double> diskUsageHistory;
};

int main()
{
    DiskMonitor diskMonitor;
    diskMonitor.setDiskName(L"C:\\");
    diskMonitor.getDiskSize();
    diskMonitor.getDiskType();
    diskMonitor.getDiskUsage();
    diskMonitor.getDiskUsageHistory();
    diskMonitor.printDiskSize();
    diskMonitor.printDiskType();
    diskMonitor.printDiskUsage();
    return 0;
}





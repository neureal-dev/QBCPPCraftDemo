#include <algorithm>
#include <assert.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

/**

    Represents a Record Object

*/

struct QBRecord {

    uint32_t column0; // unique id column

    int32_t column2;

    std::string column1;

    std::string column3;

};

/**

    Represents a Record Collections

*/

typedef std::vector<QBRecord> QBRecordCollection;

/**

    Return records that contains a string in the StringValue field

    records - the initial set of records to filter

    matchString - the string to search for

*/

QBRecordCollection QBFindMatchingRecords(const QBRecordCollection& records, const std::string& columnName, const std::string& matchString)
{
    QBRecordCollection result;

    std::function<bool(const QBRecord& rec)> selector = [=](const QBRecord& rec) -> bool { return false; };

    if (columnName == "column0") {
        uint32_t matchValue = std::stoul(matchString);
        selector = [=](const QBRecord& rec) -> bool {
            return (matchValue == rec.column0);
        };
    } else if (columnName == "column1") {
        selector = [&](const QBRecord& rec) -> bool {
            return rec.column1.find(matchString) != std::string::npos;
        };
    } else if (columnName == "column2") {
        long matchValue = std::stol(matchString);
        selector = [=](const QBRecord& rec) -> bool {
            return matchValue == rec.column2;
        };
    } else if (columnName == "column3") {
        selector = [&](const QBRecord& rec) -> bool {
            return rec.column3.find(matchString) != std::string::npos;
        };
    }

    std::copy_if(std::begin(records), std::end(records), std::back_inserter(result), selector);

    return result;
}

/**

    Comparisson value template for search QBFindMatchingRecords function

*/

template <class T, typename V>
struct QBValueMatch {

    bool operator()(const T& rec)
    {
        return (rec.*m_ == v_);
    }

    V T::* m_;

    const V& v_;
};

/**

    Comparisson string value template specialization for search QBFindMatchingRecords function

*/

template <class T>
struct QBValueMatch<T, std::string> {

    bool operator()(const T& rec)
    {
        return ((rec.*m_).find(v_) != std::string::npos);
    }

    std::string T::* m_;

    const std::string& v_;
};

/**

    Return records that contains mach value of the field

    records - the initial set of records to filter

    column - struct relative member address of the seach element &A::a

    matchValue - the value to search for

*/

template <typename ColumnType>
QBRecordCollection QBFindMatchingRecords(const QBRecordCollection& records, ColumnType QBRecord::* column, const ColumnType& matchValue)
{
    QBRecordCollection result;

    std::copy_if(std::begin(records),
        std::end(records),
        std::back_inserter(result),
        QBValueMatch<QBRecord, ColumnType>{column, matchValue});

    return result;
}

/**

    Remove a record from collection by id

    records - the initial set of records

    key - search key of the record to be removed

*/

void QBDeleteRecordById(QBRecordCollection& records, decltype(QBRecord::column0) value)
{
    /*/
    records.erase(std::remove_if(
        std::begin(records),
        std::end(records),
        [=](const QBRecordCollection::value_type& rec) -> bool {
            return rec.column0 == value;
        }),
        std::end(records));
    /*/
    auto itr = std::find_if(std::begin(records), std::end(records),
        [=](const QBRecordCollection::value_type& rec) -> bool {
            return rec.column0 == value;
        });
    if (itr != std::end(records)) {
        std::iter_swap(itr, std::prev(std::end(records)));
        records.erase(std::prev(std::end(records)));
    }
    //*/
}

/**

    Utility to populate a record collection

    prefix - prefix for the string value for every record

    numRecords - number of records to populate in the collection

*/

QBRecordCollection populateDummyData(const std::string& prefix, std::size_t numRecords)
{
    QBRecordCollection data;

    data.reserve(numRecords);

    for (std::size_t i = 0; i < numRecords; i++) {
        QBRecord rec = { static_cast<uint32_t>(i),
            static_cast<int32_t>(i % 100),
            prefix + std::to_string(i),
            std::to_string(i) + prefix };
        data.emplace_back(std::move(rec));
    }

    return data;
}

int main(int argc, char * argv[])
{
    using namespace std::chrono;
    using namespace std::string_literals;

    size_t executions = 1;

    QBRecordCollection data = populateDummyData("testdata"s, 1000);

    // Find a record that contains and measure the perf
    double avg_time = 0.0;

    for (size_t i = 0; i < executions; i++) {

        auto startTimer = steady_clock::now();

//    auto filteredSet = QBFindMatchingRecords(data, "column1", "testdata50");
//    auto filteredSet2 = QBFindMatchingRecords(data, "column2", "24");

        auto filteredSet = QBFindMatchingRecords(data, &QBRecord::column1, "testdata50"s);

        auto filteredSet2 = QBFindMatchingRecords(data, &QBRecord::column2, 24);

        avg_time += double((steady_clock::now() - startTimer).count()) * steady_clock::period::num / steady_clock::period::den;

    }

    avg_time /= executions;

    std::cout << "profiler avg: " << avg_time << std::endl;


    // make sure that the function is correct

    auto filteredSet = QBFindMatchingRecords(data, &QBRecord::column1, "testdata50"s);

    assert(data.size() == 1000);

    assert(filteredSet.size() == 11);

    auto filteredSet3 = QBFindMatchingRecords(data, &QBRecord::column0, 100u);

    QBDeleteRecordById(data, 100);

    assert(filteredSet3.size() == 1);

    filteredSet3 = QBFindMatchingRecords(data, &QBRecord::column0, 100u);

    assert(filteredSet3.size() == 0);

    assert(data.size() == 999);

    QBDeleteRecordById(data, 100);

    assert(data.size() == 999);

    std::cin.ignore();

    return 0;
}

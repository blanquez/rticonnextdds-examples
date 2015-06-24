/*******************************************************************************
 (c) 2005-2015 Copyright, Real-Time Innovations, Inc.  All rights reserved.
 RTI grants Licensee a license to use, modify, compile, and create derivative
 works of the Software.  Licensee has the right to distribute object form only
 for use with RTI products.  The Software is provided "as is", with no warranty
 of any type, including any warranty for fitness for any purpose. RTI is under
 no obligation to maintain or support the Software.  RTI shall not be liable for
 any incidental or consequential damages arising out of the use or inability to
 use the software.
 ******************************************************************************/

#include <cstdlib>   // For rand
#include <ctime>

#include "querycondition.hpp"
#include <dds/dds.hpp>

using namespace dds::core;
using namespace dds::core::policy;
using namespace dds::domain;
using namespace dds::topic;
using namespace dds::pub;
using namespace dds::pub::qos;

void publisher_main(int domain_id, int sample_count)
{
    // Create a DomainParticipant with default Qos.
    DomainParticipant participant (domain_id);

    // Create a Topic -- and automatically register the type.
    Topic<querycondition> topic (participant, "Example querycondition");

    // Retrieve the default DataWriter QoS, from USER_QOS_PROFILES.xml
    DataWriterQos writer_qos = QosProvider::Default().datawriter_qos();

    // If you want to change the DataReader's QoS programmatically rather than
    // using the XML file, uncomment the following lines.
    // writer_qos << Reliability::Reliable()
    //            << History::KeepAll();

    // Create a DataWriter.
    DataWriter<querycondition> writer(
        Publisher(participant),
        topic,
        writer_qos);

    querycondition instance;

    // Initialize random seed before entering the loop.
    srand(time(NULL));

    // TODO: Something with time
    std::string my_ids[] = {"GUID1", "GUID2", "GUID3"};

    // Main loop
    for (int count = 0; (sample_count == 0) || (count < sample_count); count++){
        // Set id to be one of the three options
        instance.id(my_ids[count % 3]);

        // Set value to a random number between 0 and 9
        instance.value((int)(std::rand() / (RAND_MAX / 10.0)));

        std::cout << "Writing querycondition, count " << count
                  << ", id = " << instance.id() << ", value = "
                  << instance.value() << std::endl;

        writer.write(instance);
        rti::util::sleep(Duration(1));
    }
}

int main(int argc, char *argv[])
{
    int domain_id = 0;
    int sample_count = 0; // Infinite loop

    if (argc >= 2) {
        domain_id = atoi(argv[1]);
    }

    if (argc >= 3) {
        sample_count = atoi(argv[2]);
    }

    // To turn on additional logging, include <rti/config/Logger.hpp> and
    // uncomment the following line:
    // rti::config::Logger::instance().verbosity(rti::config::Verbosity::STATUS_ALL);

    try {
        publisher_main(domain_id, sample_count);
    } catch (std::exception ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
        return -1;
    }

    return 0;
}

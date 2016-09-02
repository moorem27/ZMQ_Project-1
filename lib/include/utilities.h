#ifndef MESSAGE_UTILITIES_H
#define MESSAGE_UTILITIES_H

#include <string>
#include <type_traits>
#include <iostream>
#include <zmq.hpp>
#include "protobuf/messages.pb.h" // you'll have to change this to CSA_Infrastruture.pb.h

namespace util {

//----------------------------------------------------------------------------------------------------------------------
//                                            Constants
//----------------------------------------------------------------------------------------------------------------------

    constexpr std::size_t MAX_MESSAGES{10000};

//----------------------------------------------------------------------------------------------------------------------
//                                         Message Generators
//----------------------------------------------------------------------------------------------------------------------

    inline gpb::Test_Message test_message( const uint32_t num, const std::string &str) {
        gpb::Test_Message message{};
        message.set_num(num);
        message.set_str(str);
        return message;

    }

    inline bool operator==(const gpb::Test_Message &l, const gpb::Test_Message &r) {
        return l.num() == r.num() && l.str() == r.str();
    }

    inline std::ostream &operator<<(std::ostream &os, const gpb::Test_Message &message) {
        return os << "Test_Message: num=" << message.num() << ", str=" << message.str();
    }

//----------------------------------------------------------------------------------------------------------------------
//                                          Template Aliases
//----------------------------------------------------------------------------------------------------------------------

    template<bool Condition, typename T = void>
    using Enable_If = typename std::enable_if<Condition, T>::type;

//----------------------------------------------------------------------------------------------------------------------
//                                          Convenience Functions
//----------------------------------------------------------------------------------------------------------------------

    template<typename T, typename U>
    constexpr bool Is_Base_Of() { return std::is_base_of<T, U>::value; }

//----------------------------------------------------------------------------------------------------------------------
//                                           Pack Functions
//----------------------------------------------------------------------------------------------------------------------

    // primary template; we plan on packing to zmq messages, so I've made it the default
    template<typename T = zmq::message_t>
    T pack(const google::protobuf::Message &gpb_msg);

    // pack gpb -> top level gpb
    template<>
    inline gpb::Message pack<gpb::Message>(const google::protobuf::Message &gpb_msg) {
        gpb::Message packed_message{};
        packed_message.set_name(gpb_msg.GetTypeName());
        packed_message.set_encoded_message(gpb_msg.SerializeAsString());
        packed_message.CheckInitialized();
        return packed_message;
    }

    // primary template specialization
    // pack gpb -> zmq
    template<>
    inline zmq::message_t pack<zmq::message_t>(const google::protobuf::Message &gpb_msg) {
        const gpb::Message packed_gpb_msg = pack<gpb::Message>(gpb_msg);
        const std::string serialized_data = packed_gpb_msg.SerializeAsString();
        return zmq::message_t{serialized_data.begin(), serialized_data.end()};
    }

//----------------------------------------------------------------------------------------------------------------------
//                                           Unpack Functions
//----------------------------------------------------------------------------------------------------------------------

    // unpack zmq -> top level gpb
    inline gpb::Message unpack_to_top_level(const zmq::message_t& zmq_msg) {
        gpb::Message csa_msg{};
        csa_msg.ParseFromArray( zmq_msg.data(), static_cast<int>( zmq_msg.size() ) );
        csa_msg.CheckInitialized();
        return csa_msg;
    }

    // unpack zmq -> gpb
    template<typename T,
            typename = Enable_If<Is_Base_Of<google::protobuf::Message, T>()> >
    inline T unpack( const zmq::message_t& zmq_msg ) {
        gpb::Message csa_msg = unpack_to_top_level( zmq_msg );
        char* encoded_message = const_cast<char*>( csa_msg.encoded_message().c_str() );
        T unpacked_msg{};
        assert( unpacked_msg.GetTypeName() == csa_msg.name() );
        unpacked_msg.ParseFromArray( static_cast<void*>( encoded_message), csa_msg.ByteSize() );
        unpacked_msg.CheckInitialized();
        return unpacked_msg;
    }

}

#endif //MESSAGE_UTILITIES_H

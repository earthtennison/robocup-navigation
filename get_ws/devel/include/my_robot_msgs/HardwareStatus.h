// Generated by gencpp from file my_robot_msgs/HardwareStatus.msg
// DO NOT EDIT!


#ifndef MY_ROBOT_MSGS_MESSAGE_HARDWARESTATUS_H
#define MY_ROBOT_MSGS_MESSAGE_HARDWARESTATUS_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace my_robot_msgs
{
template <class ContainerAllocator>
struct HardwareStatus_
{
  typedef HardwareStatus_<ContainerAllocator> Type;

  HardwareStatus_()
    : temperature(0)
    , are_motors_up(false)
    , debug_message()  {
    }
  HardwareStatus_(const ContainerAllocator& _alloc)
    : temperature(0)
    , are_motors_up(false)
    , debug_message(_alloc)  {
  (void)_alloc;
    }



   typedef int64_t _temperature_type;
  _temperature_type temperature;

   typedef uint8_t _are_motors_up_type;
  _are_motors_up_type are_motors_up;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _debug_message_type;
  _debug_message_type debug_message;





  typedef boost::shared_ptr< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> const> ConstPtr;

}; // struct HardwareStatus_

typedef ::my_robot_msgs::HardwareStatus_<std::allocator<void> > HardwareStatus;

typedef boost::shared_ptr< ::my_robot_msgs::HardwareStatus > HardwareStatusPtr;
typedef boost::shared_ptr< ::my_robot_msgs::HardwareStatus const> HardwareStatusConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::my_robot_msgs::HardwareStatus_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::my_robot_msgs::HardwareStatus_<ContainerAllocator1> & lhs, const ::my_robot_msgs::HardwareStatus_<ContainerAllocator2> & rhs)
{
  return lhs.temperature == rhs.temperature &&
    lhs.are_motors_up == rhs.are_motors_up &&
    lhs.debug_message == rhs.debug_message;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::my_robot_msgs::HardwareStatus_<ContainerAllocator1> & lhs, const ::my_robot_msgs::HardwareStatus_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace my_robot_msgs

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> >
{
  static const char* value()
  {
    return "78c14d0acfcb188b1e76c88d6e81f318";
  }

  static const char* value(const ::my_robot_msgs::HardwareStatus_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x78c14d0acfcb188bULL;
  static const uint64_t static_value2 = 0x1e76c88d6e81f318ULL;
};

template<class ContainerAllocator>
struct DataType< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> >
{
  static const char* value()
  {
    return "my_robot_msgs/HardwareStatus";
  }

  static const char* value(const ::my_robot_msgs::HardwareStatus_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> >
{
  static const char* value()
  {
    return "int64 temperature\n"
"bool are_motors_up\n"
"string debug_message\n"
;
  }

  static const char* value(const ::my_robot_msgs::HardwareStatus_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.temperature);
      stream.next(m.are_motors_up);
      stream.next(m.debug_message);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct HardwareStatus_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::my_robot_msgs::HardwareStatus_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::my_robot_msgs::HardwareStatus_<ContainerAllocator>& v)
  {
    s << indent << "temperature: ";
    Printer<int64_t>::stream(s, indent + "  ", v.temperature);
    s << indent << "are_motors_up: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.are_motors_up);
    s << indent << "debug_message: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.debug_message);
  }
};

} // namespace message_operations
} // namespace ros

#endif // MY_ROBOT_MSGS_MESSAGE_HARDWARESTATUS_H

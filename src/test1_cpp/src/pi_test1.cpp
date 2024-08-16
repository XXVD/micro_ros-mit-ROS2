#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/int32_multi_array.hpp>

class RaspberryPiNode : public rclcpp::Node
{
public:
    RaspberryPiNode() : Node("raspberry_pi_node")
    {
        // 初始化发布者，发布到 "pi_to_pico_topic"
        publisher_ = this->create_publisher<std_msgs::msg::Int32MultiArray>("pi_to_pico_topic", 10);
        
        // 初始化订阅者，订阅 "pico_to_pi_topic"
        subscription_ = this->create_subscription<std_msgs::msg::Int32MultiArray>(
            "pico_to_pi_topic", 10, std::bind(&RaspberryPiNode::topic_callback, this, std::placeholders::_1));
        
        // 定时器，每3秒钟调用一次 timer_callback
        timer_ = this->create_wall_timer(
            std::chrono::seconds(3), std::bind(&RaspberryPiNode::timer_callback, this));
        
        // 初始化消息内容
        msg_.data.resize(4, 0);
    }

private:
    void timer_callback()
    {
        // 每次调用时，将消息中的数据元素增加
        msg_.data[0]++;
        msg_.data[1]++;
        msg_.data[2]++;
        msg_.data[3]++;
        publisher_->publish(msg_);
    }

    void topic_callback(const std_msgs::msg::Int32MultiArray::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Received from Pico: [%d, %d, %d, %d]",
                    msg->data[0], msg->data[1], msg->data[2], msg->data[3]);
    }

    rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr publisher_;
    rclcpp::Subscription<std_msgs::msg::Int32MultiArray>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
    std_msgs::msg::Int32MultiArray msg_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RaspberryPiNode>());
    rclcpp::shutdown();
    return 0;
}

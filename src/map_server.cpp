#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <nav_msgs/GetMap.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>

// compute linear index for given map coords
#define MAP_IDX(sx, i, j) ((sx) * (j) + (i))

class map_loader
{
private:
    ros::NodeHandle nh;
    ros::Publisher map_pub;
    ros::ServiceServer service;

    std::string filename;
    std::string frame_id;
    int map_size_x;
    int map_size_y;
    double map_resolution;
    double origin_x;
    double origin_y;

public:
    map_loader(/* args */);
    ~map_loader();
    bool pcmapservice(nav_msgs::GetMap::Request &req, nav_msgs::GetMap::Response &res);
    void set_filename(std::string filename_){this->filename = filename_;};
};

map_loader::map_loader(/* args */)
{
    //tomikawa add
    if(!nh.getParam("frame_id", frame_id))
        frame_id = "map";
    if(!nh.getParam("width", map_size_x))
        map_size_x = 384;
    if(!nh.getParam("height", map_size_y))
        map_size_y = 384;
    if(!nh.getParam("resolution", map_resolution))
        map_resolution = 0.05;
    if(!nh.getParam("origin_x", origin_x))
        origin_x = -10.0;
    if(!nh.getParam("origin_y", origin_y))
        origin_y = -10.0;

    service = nh.advertiseService("static_map", &map_loader::pcmapservice, this);

}

map_loader::~map_loader()
{
}


bool map_loader::pcmapservice(nav_msgs::GetMap::Request &req,
                             nav_msgs::GetMap::Response &res)
{

    pcl::PointCloud<pcl::PointXYZRGB> point_cloud;

    ROS_INFO("Start load map ...");
    ROS_INFO("filename:=%3s", filename.c_str());
    if (pcl::io::loadPCDFile<pcl::PointXYZRGB> (filename, point_cloud) == -1) 
    {
        ROS_ERROR("Couldn't read file %s \n", filename.c_str());
        return false;
    }
    ROS_INFO("Load map successful.");

    ROS_INFO("Start Setting Map!");
    
    unsigned int index = 0;
    res.map.header.frame_id = frame_id;
    res.map.info.width = map_size_x;
    res.map.info.height = map_size_y;
    res.map.info.resolution = map_resolution;
    res.map.info.origin.position.x = origin_x;
    res.map.info.origin.position.y = origin_y;
    res.map.data.resize(map_size_x * map_size_y);

    for (unsigned int i = 0; i < map_size_x; i++)
    {
        for (unsigned int j = 0; j < map_size_y; j++)
        { 
            if (point_cloud.points[MAP_IDX(map_size_x, i, j)].r == 0 && point_cloud.points[MAP_IDX(map_size_x, i, j)].g == 0 && point_cloud.points[MAP_IDX(map_size_x, i, j)].b == 0 && point_cloud.points[MAP_IDX(map_size_x, i, j)].a == 255 )
                res.map.data[index] = 100;
            else if (point_cloud.points[MAP_IDX(map_size_x, i, j)].r == 255 && point_cloud.points[MAP_IDX(map_size_x, i, j)].g == 255 && point_cloud.points[MAP_IDX(map_size_x, i, j)].b == 255 && point_cloud.points[MAP_IDX(map_size_x, i, j)].a == 255)
                res.map.data[index] = 0;
            else if(point_cloud.points[MAP_IDX(map_size_x, i, j)].r == 155 && point_cloud.points[MAP_IDX(map_size_x, i, j)].g == 155 && point_cloud.points[MAP_IDX(map_size_x, i, j)].b == 155 && point_cloud.points[MAP_IDX(map_size_x, i, j)].a == 255)    
                res.map.data[index] = -1;

            ++index;

        }
    }

    ROS_INFO("Successful Set Map!");

    return true;

}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "point_cloud_map_server");
    ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug);

    // コマンドライン引数が0個or2個以上の場合
    // if (argc != 2) {
    //     // 指定するのは出力したい.pcdファイルのみ
    //     ROS_ERROR("Error command!");
    //     return 1;
    // }

    map_loader ml;
    ml.set_filename(argv[1]);

    ROS_INFO("Start Map Server!");

    ros::spin();

    return 0;
}

# point_cloud_map_server
## Details
このパッケージは、３つのノードによって構成されている
### Node
* map_loader  
.pcdファイルを読み込み、sensor_msgs/PointCloud2の型に変換し、"/point_cloud_map"というtopicをpublishするノード（rvizで可視化するとpubされていることがわかる） 
* map_saver    
.pcdファイルを保存するノード
* map_server
.pcdファイルを読み込み、nav_msgs/GetMapのサービスの方に変換しtrueで返すノード（amclノードに点群情報を渡すことができるノード）

## Build
  ```shell 
  cd ${catkin_workspace}/src
  git clone https://github.com/ningwang1028/point_cloud_map_server.git
  cd ..
  catkin_build
  ```
## Run 
* save map
  ```shell  
  rosrun point_cloud_map_server map_saver file_name
  ```
* load map
    ```shell  
  rosrun point_cloud_map_server map_saver xxx.pcd 
  ```
* server map
    ```shell  
  rosrun point_cloud_map_server map_sever xxx.pcd 
  ```
なお、pointcloudmapの中に.pcdファイルが入っているため、上記のノード起動の動作確認の際に用いると良い

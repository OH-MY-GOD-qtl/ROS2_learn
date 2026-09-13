import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/qtl/ros2book/chapt2/chapt2_ws/install/demo_python_pkg'

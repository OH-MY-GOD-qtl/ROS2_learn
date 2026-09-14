import rclpy

from demo_python_pkg.person_node import PersonNode

class WriteNode(PersonNode):
    def __init__(self, name:str, age:int, book:str) -> None:
        super().__init__(name, age)
        print('WriteNode 的 __init__() 方法被调用了')
        self.book = book

def main():
    rclpy.init()
    node = WriteNode('法外狂徒张三', 18, '论持久战')
    node.eat('鱼香肉丝')
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()
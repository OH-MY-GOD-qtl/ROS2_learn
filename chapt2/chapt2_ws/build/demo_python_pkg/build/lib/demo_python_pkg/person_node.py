import rclpy
from rclpy.node import Node

class PersonNode(Node):
    def __init__(self, name:str, age:int) -> None:
        super().__init__(node_name='person_node')
        self.age = age
        self.name = name

    def eat(self, food_name:str):
        print(f'{self.name} is {self.age} years old and is eating {food_name}')

def main():
    rclpy.init()
    node = PersonNode('法外狂徒张三', 18)
    node.eat('鱼香肉丝')
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()
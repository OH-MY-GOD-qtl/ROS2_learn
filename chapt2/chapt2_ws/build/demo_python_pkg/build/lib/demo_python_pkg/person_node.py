class PersonNode:
    def __init__(self, name:str, age:int) -> None:
        print('PersonNode 的 __init__() 方法被调用了')
        self.age = age
        self.name = name

    def eat(self, food_name:str):
        print(f'{self.name} is {self.age} years old and is eating {food_name}')

def main():
    node = PersonNode('法外狂徒张三', 18)
    node.eat('鱼香肉丝')
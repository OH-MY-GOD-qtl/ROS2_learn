import os
import sys

import cv2

try:
    from ament_index_python.packages import get_packages_share_directory
except ImportError:
    get_packages_share_directory = None


def get_default_image_path():
    """获取 default.png 的路径。

    优先使用 ROS2 安装后的 share 目录，失败时回退到源码目录，
    这样脚本既能在安装后运行，也能直接运行。
    """
    if get_packages_share_directory is not None:
        try:
            image_path = os.path.join(
                get_packages_share_directory('demo_python_service'),
                'resource', 'default.png')
            if os.path.exists(image_path):
                return image_path
        except Exception:
            pass

    # 回退：源码目录
    return os.path.join(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
        'resource', 'default.png')


def get_cascade_path():
    """查找 haarcascade_frontalface_default.xml 模型文件的路径。

    pip 版 OpenCV 自带 cv2.data，apt 版则放在 /usr/share/opencv4 下，
    这里依次尝试，保证两种环境都能找到。
    """
    candidates = []
    if hasattr(cv2, 'data') and cv2.data.haarcascades:
        candidates.append(
            os.path.join(cv2.data.haarcascades,
                         'haarcascade_frontalface_default.xml'))
    candidates.append(
        '/usr/share/opencv4/haarcascades/'
        'haarcascade_frontalface_default.xml')

    for path in candidates:
        if os.path.exists(path):
            return path
    return None


def detect_faces(image):
    """使用 Haar Cascade 检测图像中的人脸。

    返回检测框列表，每个元素为 (x, y, w, h)。
    """
    cascade_path = get_cascade_path()
    if cascade_path is None:
        raise FileNotFoundError('未找到 haarcascade_frontalface_default.xml')

    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    cascade = cv2.CascadeClassifier(cascade_path)
    faces = cascade.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=5,
        minSize=(30, 30))
    return faces


def main():
    image_path = get_default_image_path()
    image = cv2.imread(image_path)
    if image is None:
        print(f'无法读取图片: {image_path}')
        return 1

    faces = detect_faces(image)
    print(f'共检测到 {len(faces)} 张人脸')

    for (x, y, w, h) in faces:
        cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 2)

    output_path = os.path.join(os.path.dirname(image_path), 'face_result.png')
    cv2.imwrite(output_path, image)
    print(f'检测结果已保存到: {output_path}')

    # 尝试弹窗显示，无 GUI 环境（如 SSH）下自动跳过
    try:
        cv2.imshow('Face Detection', image)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
    except cv2.error:
        pass

    return 0


if __name__ == '__main__':
    sys.exit(main())

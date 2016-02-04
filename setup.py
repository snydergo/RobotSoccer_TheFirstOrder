## ! DO NOT MANUALLY INVOKE THIS setup.py, USE CATKIN INSTEAD

from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

# fetch values from package.xml
setup_args = generate_distutils_setup(
	name='robot_soccer',
    packages=['visiondata', 'Gameplay', 'roboclaw_python', "main_control"],
    package_dir={'': 'AI', '': 'motion_control'},
)

setup(**setup_args)
#package_dir={'': 'src'},

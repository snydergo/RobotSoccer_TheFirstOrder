## ! DO NOT MANUALLY INVOKE THIS setup.py, USE CATKIN INSTEAD

from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

# fetch values from package.xml
"""setup_args = generate_distutils_setup(
	name='robot_soccer',
    packages=['visiondata', 'Gameplay', 'maincontrol'],
    package_dir={'': 'AI'},
)
"""

setup_args = generate_distutils_setup(
	name='robot_soccer',
    packages=['roboclaw_python', 'library'],
    package_dir={'': 'motion_control'},
)

setup(**setup_args)
#package_dir={'': 'src'},

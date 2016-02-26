import rospy
from std_msgs.msg import String
from getch import _Getch
from robot_soccer.msg import piddata

# Default commands
class Tuning:
    def __init__(self):
      self.x_cmd = 0
      self.y_cmd = 0
      self.theta_cmd = 0

      self.kp_x   = .02
      self.ki_x   = .00001
      self.kd_x   = .000

      self.kp_y   = .02
      self.ki_y   = .00001
      self.kd_y   = .000

      self.kp_theta  = .02
      self.ki_theta  = .00005
      self.kd_theta  = 0

      self.loop_rate = 10 # Hz (get value from publisher)
      self.tuningVariable = 'x'
      self.tuningSensitivity = .001

T = Tuning()


def get_action():
    print("getting action")
    getch = _Getch()
    pressed = getch()
    print("called getch")
    # Show commands (help)
    if pressed == 'h':
        return 'HELP'
    elif pressed == 'q':
        return 'QUIT'
    # Switch tuning variable
    elif pressed == '1':
        return 'X'
    elif pressed == '2':
        return 'Y'
    elif pressed == '3':
        return 'THETA'

    # change tuning paramaters for current tuning variable
    elif pressed == ',':
        return 'KP_DOWN'
    elif pressed == '.':
        return 'KP_UP'
    elif pressed == 'k':
        return 'KD_DOWN'
    elif pressed == 'l':
        return 'KD_UP'
    elif pressed == 'i':
        return 'KI_DOWN'
    elif pressed == 'o':
        return 'KI_UP'

    # Tools
    elif pressed == 'l':
        return 'LIST'
    elif pressed == 's':
        return 'SENSITIVITY'

    # change cmd position
    elif pressed == ' ':
        return 'CHANGE_CMD_POS'
    
    else:
        return 'NOTHING'

def do_action():
    message = piddata()
    action = get_action()
    # Display commands (help)
    if action == 'HELP':
        print("Change tuning variable with 1,2,3")
        print("Tune kp with < >")
        print("Tune kd with k l")
        print("Tune ki with i o")
        print("Press l to show current gains")
        print("Press s to change tuning sensitivity")
        print("Press Space to change commanded position")
        print("Press h to bring up the commands (this menu)")
        print("Press q to quit")

    elif action == 'QUIT':
        quit()

    # Switch tuning variable
    elif action == 'X':
        T.tuningVariable = 'x'
        print("changed tuning variable to x")
    elif action == 'Y':
        T.tuningVariable = 'y'
        print("changed tuning variable to y")
    elif action == 'THETA':
        T.tuningVariable = 'theta'
        print("changed tuning variable to theta")

    # change tuning paramaters for current tuning variable
    elif action == 'KP_DOWN':
        if T.tuningVariable == 'x':
            T.kp_x = T.kp_x - T.tuningSensitivity
            print("kp_x: " + str(T.kp_x))
        elif T.tuningVariable == 'y':
            T.kp_y = T.kp_y - T.tuningSensitivity
            print("kp_y: " + str(T.kp_y))
        elif T.tuningVariable == 'theta':
            T.kp_theta = T.kp_theta - T.tuningSensitivity
            print("kp_theta: " + str(T.kp_theta))
    elif action == 'KP_UP':
        if T.tuningVariable == 'x':
            T.kp_x = T.kp_x + T.tuningSensitivity
            print("kp_x: " + str(T.kp_x))
        elif T.tuningVariable == 'y':
            T.kp_y = T.kp_y + T.tuningSensitivity
            print("kp_y: " + str(T.kp_y))
        elif T.tuningVariable == 'theta':
            T.kp_theta = T.kp_theta + T.tuningSensitivity
            print("kp_theta: " + str(T.kp_theta))  
    elif action == 'KD_DOWN':
        if T.tuningVariable == 'x':
            T.kd_x = T.kd_x - T.tuningSensitivity
            print("kd_x: " + str(T.kd_x))
        elif T.tuningVariable == 'y':
            T.kd_y = T.kd_y - T.tuningSensitivity
            print("kd_y: " + str(T.kd_y))
        elif T.tuningVariable == 'theta':
            T.kd_theta = T.kd_theta - T.tuningSensitivity
            print("kd_theta: " + str(T.kd_theta))
    elif action == 'KD_UP':
        if T.tuningVariable == 'x':
            T.kd_x = T.kd_x + T.tuningSensitivity
            print("kd_x: " + str(T.kd_x))
        elif T.tuningVariable == 'y':
            T.kd_y = T.kd_y + T.tuningSensitivity
            print("kd_y: " + str(T.kd_y))
        elif T.tuningVariable == 'theta':
            T.kd_theta = T.kd_theta + T.tuningSensitivity
            print("kd_theta: " + str(T.kd_theta))       
    elif action == 'KI_DOWN':
        if T.tuningVariable == 'x':
            T.ki_x = T.ki_x - T.tuningSensitivity
            print("ki_x: " + str(T.ki_x))
        elif T.tuningVariable == 'y':
            T.ki_y = T.ki_y - T.tuningSensitivity
            print("ki_y: " + str(T.ki_y))
        elif T.tuningVariable == 'theta':
            T.ki_theta = T.ki_theta - T.tuningSensitivity
            print("ki_theta: " + str(T.ki_theta))       
    elif action == 'KI_UP':
        if T.tuningVariable == 'x':
            T.ki_x = T.ki_x + T.tuningSensitivity
            print("ki_x: " + str(T.ki_x))
        elif T.tuningVariable == 'y':
            T.ki_y = T.ki_y + T.tuningSensitivity
            print("ki_y: " + str(T.ki_y))
        elif T.tuningVariable == 'theta':
            T.ki_theta = T.ki_theta + T.tuningSensitivity
            print("ki_theta: " + str(T.ki_theta)) 


    elif action == 'LIST':
        print("kp_x: " + str(T.kp_x))
        print("kd_x: " + str(T.kd_x))
        print("ki_x: " + str(T.ki_x))
        print("\n")
        print("kp_y: " + str(T.kp_y))
        print("kd_y: " + str(T.kd_y))
        print("ki_y: " + str(T.ki_y))
        print("\n")
        print("kp_theta: " + str(T.kp_theta))
        print("kd_theta: " + str(T.kd_theta))
        print("ki_theta: " + str(T.ki_theta))
    elif action == 'SENSITIVITY':
        T.tuningSensitivity = input("Tuning Sensitivity: ")

    
def pid_tuner():
  pub = rospy.Publisher('robot1Com', String, queue_size=10)
  rospy.init_node('talker', anonymous=True)
  rate = rospy.Rate(10) # 10hz
  while not rospy.is_shutdown():
      do_action()
      message = piddata()
      for i in vars(T):
            if i in vars(message):
                setattr(message, i, eval('T.{}'.format(i)))
      message.cmdType = 'pid'
      rospy.loginfo(message.cmdType)
      pub.publish(message)
      rate.sleep()

if __name__ == '__main__':
   try:
       pid_tuner()
   except rospy.ROSInterruptException:
       pass
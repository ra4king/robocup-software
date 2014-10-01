import play
import behavior
import skills.move
import robocup
import constants

class TeeUp(play.Play):
    def __init__(self):
        super().__init__(continuous=False)

        robot_size = constants.Robot.Radius * 2.0

        p = robocup.Point(0, constants.Field.Length / 2.0)
        self.add_subbehavior(skills.move.Move(p), name="robot1")

        p = robocup.Point(-robot_size * 3, constants.Field.Length / 2.0)
        self.add_subbehavior(skills.move.Move(p), name="robot2")

        p = robocup.Point(robot_size * 3, constants.Field.Length / 2.0)
        self.add_subbehavior(skills.move.Move(p), name="robot3")

        p = robocup.Point(0, constants.Field.Length / 2.0 - robot_size * 2.0)
        self.add_subbehavior(skills.move.Move(p), name="robot4")

        p = robocup.Point(0, constants.Field.Length / 2.0 - robot_size * 4.0)
        self.add_subbehavior(skills.move.Move(p), name="robot5")

        p = robocup.Point(0, constants.Field.Length / 2.0 - robot_size * 6.0)
        self.add_subbehavior(skills.move.Move(p), name="robot6")

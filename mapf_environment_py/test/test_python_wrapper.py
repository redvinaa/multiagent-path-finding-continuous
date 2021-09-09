#!/usr/bin/env python3.6
# Copyright 2021 Reda Vince


import os
import sys
import unittest
import rospkg
from mapf_env import Environment as Env
from mapf_env import Point
import rosunit

__author__ = "Reda Vince"
__copyright__ = "Copyright 2021 Reda Vince"

pkg = 'mapf_environment'


def make_env():
    pkg_path = rospkg.RosPack().get_path(pkg)
    img_path = os.path.join(pkg_path, 'maps', 'test_4x4.jpg')

    return Env(img_path, 2)


class TestWrapper(unittest.TestCase):

    def test_point(self):
        p = Point()
        self.assertEqual(p.x, 0)
        self.assertEqual(p.y, 0)
        self.assertEqual(p.z, 0)
        p.x = 1
        p.y = 0.5
        p.z = -3
        self.assertEqual(p.x, 1)
        self.assertEqual(p.y, 0.5)
        self.assertEqual(p.z, -3)

    def test_env(self):

        # constructor
        env = make_env()

        # reset
        env_obs = env.reset()
        self.assertFalse(env.is_done())

        # render && step
        from mapf_env import Point
        while not env_obs.done:
            env.render(10)
            p1 = Point()
            p1.x = 1.
            p2 = Point()
            p2.x = 1.
            p2.z = -0.5
            env_obs = env.step([p1, p2])

        self.assertEqual(env.get_observation_size(), 17)

if __name__ == '__main__':
    rosunit.unitrun(pkg, 'test_wrapper', TestWrapper)
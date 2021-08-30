// Copyright 2021 Reda Vince

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "mapf_environment/environment.h"
#include <opencv2/opencv.hpp>
#include <string>

namespace py = pybind11;

PYBIND11_MODULE(mapf_env, m)
{
    m.doc() = "Multi-agent path finding environment python wrapper";

    py::class_<Point>(m, "Point", "Stores a 3D point: x, y, z")
        .def(py::init<>())
        .def_readwrite("x", &Point::x)
        .def_readwrite("y", &Point::y)
        .def_readwrite("z", &Point::z);

    py::class_<Observation>(m, "Observation")
        .def_readwrite("agent_pose", &Observation::agent_pose,
            "x and y stores the position, z the angle of an agent")
        .def_readwrite("agent_twist", &Observation::agent_twist,
            "x stores the linear, z the angular velocity")
        .def_readwrite("goal_pose", &Observation::goal_pose,
            "Only x and y is used, as position")
        .def_readwrite("scan", &Observation::scan,
            "Vector to store ranges of the lidar")
        .def_readwrite("reward", &Observation::reward,
            "Reward received after the previous step");

    py::class_<EnvStep>(m, "EnvStep", "Data returned from the Environment after calling step")
        .def_readwrite("observations", &EnvStep::observations, "Observations of all the agents")
        .def_readwrite("done", &EnvStep::done, "Whether the episode is done");

    py::class_<Environment>(m, "Environment", "Environment for multi-agent path finding simulation")
    .def(py::init<std::string, float, int, float, float, float, int, int, int, int,
        bool, float, float, float, unsigned int>(),
        py::arg("_map_path"),
        py::arg("_physics_step_size")    = 0.01,
        py::arg("_step_multiply")        = 5,
        py::arg("_laser_max_angle")      = 45.*M_PI/180.,
        py::arg("_laser_max_dist")       = 10.,
        py::arg("_robot_diam")           = 0.8,
        py::arg("_velocity_iterations")  = 6,
        py::arg("_position_iterations")  = 2,
        py::arg("_render_height")        = 700,
        py::arg("_laser_nrays")          = 10,
        py::arg("_draw_laser")           = false,
        py::arg("_goal_reaching_reward") = 0.,
        py::arg("_collision_reward")     = -1.,
        py::arg("_step_reward")          = -1.,
        py::arg("_seed")                 = 0
        )
    .def("reset",        &Environment::reset,
        "Set done=false, generate new starting positions and goals for all agents")
    .def("add_agent",    &Environment::add_agent,
        "Create an agent with physics, random starting and goal positions")
    .def("remove_agent", &Environment::remove_agent,
        "Remove the given agent and its goal from the simulation")
    .def("step",         &Environment::step,
        "Add actions, get observations, rewards and done")
    .def("render",       &Environment::render,
        "Show rendered image of the environment", py::arg("wait") = 0)
    .def("is_done",      &Environment::is_done,
        "Is the episode over")
    .def("get_number_of_agents", &Environment::get_number_of_agents,
        "Get number of agents")
    .def("get_episode_sim_time", &Environment::get_episode_sim_time,
        "How much time has passed in the simulation since the start of the episode")
    .def("get_observation_size", &Environment::get_observation_size,
        "Calculate how many relevant elements an observation has")
    .def_static("serialize_observation", &Environment::serialize_observation,
        "Take the Observation structure an put the relevant data in a float vector (STRIPS REWARD)")
    .def_static("deserialize_observation", &Environment::deserialize_observation,
        "Take the observation data as a float vecto and construct an Observation object out of it (REWARD IS EMPTY)");
}

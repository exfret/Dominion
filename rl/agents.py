import random


class Agent:
    '''
    A base class for an agent.
    You might want include more class functions for tasks such as
    re-training, updating internal weights, etc.
    '''

    def __init__(self, name):
        self.name = name

    def observe(self, observation):
        '''Allow this agent to observe the given observation.'''
        raise ValueError('Not implemented yet.')

    def take_reward(self, amount):
        '''Reward this agent with a float value.'''
        raise ValueError('Not implemented yet.')

    def select_action(self, actions):
        '''Returns the index of a chosen action from a given list of actions.'''
        raise ValueError('Not implemented yet.')


class RandomAgent(Agent):
    '''An agent that selects actions at random.'''

    def __init__(self, name):
        super().__init__(name)

    def observe(self, observation):
        pass

    def take_reward(self, amount):
        pass

    def select_action(self, actions):
        return random.choice(actions)


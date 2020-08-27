from agents import RandomAgent
from environment import Environment


class Game:
    '''Dominion game wrapper.'''

    def __init__(self, env, agent_a, agent_b):
        self.env = env
        self.agent_a = agent_a
        self.agent_b = agent_b
        self.turn = 'a'
        self.total_reward_a = 0
        self.total_reward_b = 0

    def play_action(self):
        '''
        Play a single action for the current agent.
        Returns True if the game has concluded. Otherwise, returns False.
        '''

        # get all next actions (we should always see at least one action)
        actions = env.next_actions(self.turn)
        assert len(actions) > 0

        # determine who is playing and who is resting
        if self.turn == 'a':
            action = self.agent_a.select_action(actions)
        if self.turn == 'b':
            action = self.agent_b.select_action(actions)

        # step the environment
        (obs_a, obs_b), (reward_a, reward_b), next_turn = self.env.step(action)

        # provide agents with observations of what happened
        self.agent_a.observe(obs_a)
        self.agent_b.observe(obs_b)

        # provide agents with associated rewards and track globally
        self.agent_a.take_reward(reward_a)
        self.agent_b.take_reward(reward_b)
        self.total_reward_a += reward_a
        self.total_reward_b += reward_b

        # the game is not over yet
        if next_turn:
            self.turn = next_turn
            return False

        # game is finished
        return True

    def play(self):
        '''Play a whole game.'''

        done = False
        while not done:
            done = self.play_action()

        if self.total_reward_a == self.total_reward_b:
            return None
        elif self.total_reward_a > self.total_reward_b:
            return 'a'
        else:
            return 'b'


if __name__ == '__main__':
    env = Environment()
    agent_a = RandomAgent('a')
    agent_b = RandomAgent('b')
    game = Game(env, agent_a, agent_b)

    print('Playing a new game.')
    winner = game.play()
    print('Game has completed.')
    if winner:
        print('Player {} has won'.format(a))
    else:
        print('Game is a tie.')


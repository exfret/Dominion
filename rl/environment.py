from subprocess import Popen, PIPE


class Environment:
    '''Represents an environment for two learning agents.'''

    def __init__(self):
        # Initialize the game process
        cmd = ["./dominion/dominion.out"]
        self.phase = 'action'
        self.p = Popen(cmd, stdin=PIPE, stdout=PIPE)

        # Create and start a new game
        self._send_command('newgame')
        self._send_command('startgame')

    def __str__(self):
        '''Return a human-readable version of the environment.'''
        raise ValueError('Not implemented yet.')

    def next_actions(self, player_name):
        '''Return a list of valid next actions for the given player.'''
        raise ValueError('Not implemented yet.')

    def step(self, action):
        '''
        Step the environment forward in time with the given action.
        Returns a tuple containing the following values:
          - observations:
                a list of Observation objects containing information about the
                current state visible by each agent
          - rewards:
                a list of float values representing the reward each agent receives as a result
                of playing the given action
          - next_turn:
                a flag set to 'a' or 'b', depending on which agent plays next,
                or None if the game as finished.
        Raises a ValueError when the given action is not valid.
        '''
        raise ValueError('Not implemented yet.')

    def _send_command(self, command):
        '''Send command to the game process and return the output.'''
        self.p.stdin.write(bytes(command + '\n', encoding='ascii'))
        self.p.stdin.flush()
        out = self.p.stdout.readline()
        return str(out, encoding='ascii')


class Action:
    '''Action that an agent can take.'''

    def __init__(self, agent_name, name):
        assert type(name) == str
        assert agent_name in ['a', 'b']
        self.name = name
        self.agent_name = agent_name

    def __str__(self):
        '''Return a human-readable version of the action.'''
        return '{} by {}'.format(self.name, self.agent_name)

    def to_numpy(self):
        '''Encode this action as a numpy array.'''
        raise ValueError('Not implemented yet.')


class Observation:
    '''Information about what each agent can see.'''

    def __init__(self):
        pass

    def __str__(self):
        '''Return a human-readable version of the observation.'''
        return name

    def to_numpy(self):
        '''Encode this observation as a numpy array.'''
        raise ValueError('Not implemented yet.')

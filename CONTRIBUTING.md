To contribute, create an issue, and ask to be assign to (to avoid multiple work on the same topic).

Then fork or create a branch (git flow used) and proceed by pull requests.

Please create tests for your features and all the tests must pass before submitting the pull request.

## Modules
### BaseObject
Common methods to all objects.

### Character
Manage all classes related to the Character management:
- Character
- NPC
- Habilities
- ...

### Config
Classes related to the context and the configuration reading:
- Context
- Config

Contains as well the definitions of the constants following:
- config file keywords
- verbosity levels

### Database
Database management and model.
Classes:
- Database
- Query

Contains the definitions of the constants related to the database Model.

### Engine
Main file to the RPGEngine runtime.

### Events
Classes related to the events and asynchronous management:
- Event
- EventLoop (not implemented)
- Work
- WorkerThread

### Game
Classes related to the management of the run of the game:
- GameLauncher
- GameLoader
- Game

### Maker
Classes and runtime for the maker tools.

### Map
Classes to manage the map and the positions:
- Area
- Vector
- Position
- Map

### Object
Classes related to the object manipulation:
- Object
- Inventory
- Money

### Quest
Classes related to the journal and quests:
- Quest
- QuestJournal
- Dialogue

### Tools
Usefulls tools.
BaseException

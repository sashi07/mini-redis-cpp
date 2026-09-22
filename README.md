\# Mini-Redis C++



A Redis-inspired, multithreaded in-memory key-value server implemented in C++.



This project focuses on understanding the core systems concepts behind an in-memory database and network server, including TCP networking, concurrent request handling, RESP-style parsing, TTL-based expiration, append-only persistence, background snapshots, and primary-replica replication.



> This is an educational Redis-inspired implementation, not a full Redis replacement.



\---



\## Features



\- TCP client-server architecture

\- RESP-style request parsing

\- Thread-pool based client handling

\- In-memory key-value storage using `std::unordered\_map`

\- Concurrent access using `std::shared\_mutex`

\- `GET`, `SET`, and `DEL` commands

\- Key expiration using `EXPIRE`

\- Background TTL eviction

\- Append-only file persistence (AOF)

\- AOF recovery during server startup

\- Background snapshot support using `BGSAVE`

\- Primary-replica replication

\- CMake-based build system



\---



\## Architecture



```text

&#x20;                   Client

&#x20;                     |

&#x20;                     | TCP / RESP

&#x20;                     v

&#x20;              +-------------+

&#x20;              |   Server    |

&#x20;              | socket()    |

&#x20;              | bind()      |

&#x20;              | listen()    |

&#x20;              | accept()    |

&#x20;              +------+------+

&#x20;                     |

&#x20;                     v

&#x20;               Thread Pool

&#x20;                     |

&#x20;                     v

&#x20;              handle\_client()

&#x20;                     |

&#x20;                     v

&#x20;                parse\_resp()

&#x20;                     |

&#x20;                     v

&#x20;             process\_command()

&#x20;                     |

&#x20;         +-----------+-----------+

&#x20;         |           |           |

&#x20;         v           v           v

&#x20;       GET         SET/DEL     EXPIRE

&#x20;         |           |           |

&#x20;         +-----------+-----------+

&#x20;                     |

&#x20;                     v

&#x20;                In-Memory Store

&#x20;                     |

&#x20;         +-----------+-----------+

&#x20;         |           |           |

&#x20;         v           v           v

&#x20;        TTL         AOF       Replicas


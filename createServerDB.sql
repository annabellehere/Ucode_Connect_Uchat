CREATE TABLE user (
  user_id INTEGER PRIMARY KEY AUTOINCREMENT,
  user_nick TEXT NOT NULL,
  user_avathar TEXT,
  user_login TEXT NOT NULL UNIQUE,
  password TEXT NOT NULL,
  create_time DATETIME NOT NULL DEFAULT (DATETIME('now' )),
  edit_time DATETIME NOT NULL DEFAULT (DATETIME('now' ))
);


CREATE TABLE chat (
  chat_id INTEGER PRIMARY KEY AUTOINCREMENT,
  chat_type INTEGER NOT NULL,
  chat_name TEXT,
  chat_avathar TEXT,
  member_count INTEGER NOT NULL,
  create_time DATETIME NOT NULL DEFAULT (DATETIME('now' ))
);


CREATE TABLE chat_user (
  chat_id INTEGER NOT NULL,
  user_id INTEGER NOT NULL,
  FOREIGN KEY (chat_id) REFERENCES chat (chat_id),
  FOREIGN KEY (user_id) REFERENCES user (user_id)
);

CREATE TABLE message (
  chat_id INTEGER NOT NULL,
  sendtime DATETIME NOT NULL DEFAULT (DATETIME('now' )),
  text TEXT NOT NULL,
  author_id INTEGER NOT NULL,
  message_id INTEGER PRIMARY KEY AUTOINCREMENT,
  message_type INTEGER NOT NULL,
  media TEXT,
  edit_time DATETIME NOT NULL DEFAULT (DATETIME('now' )),
  FOREIGN KEY (chat_id) REFERENCES chat (chat_id),
  FOREIGN KEY (author_id) REFERENCES user (user_id)
);

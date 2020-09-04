-- Table: device
CREATE TABLE device (
    ID integer NOT NULL CONSTRAINT device_pk PRIMARY KEY AUTOINCREMENT,
    name text NOT NULL,
    type text NOT NULL,
    format text NOT NULL,
    adress text NOT NULL,
    operator_ID integer NOT NULL,
    CONSTRAINT device_operator FOREIGN KEY (operator_ID)
    REFERENCES operator (ID)
);

-- Table: operator
CREATE TABLE operator (
    ID integer NOT NULL CONSTRAINT operator_pk PRIMARY KEY AUTOINCREMENT,
    host varchar(15) NOT NULL,
    name text NOT NULL
);

-- Table: service
CREATE TABLE service (
    ID integer NOT NULL CONSTRAINT service_pk PRIMARY KEY AUTOINCREMENT,
    name text NOT NULL,
    type text NOT NULL,
    priority integer NOT NULL
);

-- Table: task
CREATE TABLE task (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    tbegin DATETIME DEFAULT CURRENT_TIMESTAMP,
    taccept DATETIME,
    tend DATETIME,
    ticket TEXT(4),
    operator_ID integer,
    service_ID integer NOT NULL,
    CONSTRAINT task_operator FOREIGN KEY (operator_ID)
    REFERENCES operator (ID),
    CONSTRAINT task_service FOREIGN KEY (service_ID)
    REFERENCES service (ID)
);

-- Table: terminal
CREATE TABLE terminal (
    ID integer NOT NULL CONSTRAINT terminal_pk PRIMARY KEY AUTOINCREMENT,
    name text NOT NULL,
    ip text NOT NULL
);

-- Tables

CREATE TABLE IF NOT EXISTS farmer (
    company_nb VARCHAR(20)
    name VARCHAR(20),
    address VARCHAR(20),
    city VARCHAR(20),
    operation_nb VARCHAR(20),
    PRIMARY KEY (company_nb),
    FOREIGN KEY (operation_nb) REFERENCES operation(operation_nb),
);

CREATE TABLE IF NOT EXISTS feed (
    storage_location VARCHAR(20)
    weight VARCHAR(20),
    type VARCHAR(20),
    PRIMARY KEY (storage_location)
);


CREATE TABLE IF NOT EXISTS operation (
    operation_nb VARCHAR(20),
    name VARCHAR(20),
    type VARCHAR(20),
    feed VARCHAR(20),
    PRIMARY KEY (operation_nb),
    FOREIGN KEY (feed) REFERENCES feed(storage_location)
);


CREATE TABLE IF NOT EXISTS herd (
    field INT,
    dog VARCHAR(20),
    farmer VARCHAR(20),
    PRIMARY KEY (field)
    FOREIGN KEY (farmer) REFERENCES farmer(company_nb)
);

CREATE TABLE IF NOT EXISTS cow (
    id_number VARCHAR(20)
    name VARCHAR(20),
    race VARCHAR(20),
    lactation_rank INT,
    daily_production INT,
    field INT,
    PRIMARY KEY (id_number),
    FOREIGN KEY (field) REFERENCES herd(field)
);






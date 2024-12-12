-- Basis-Tabellen
CREATE TABLE city_name (
    city_name_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL UNIQUE,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE zip_code (
    zip_code_id INTEGER PRIMARY KEY AUTOINCREMENT,
    code VARCHAR(10) NOT NULL UNIQUE,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE zip_city (
    zip_city_id INTEGER PRIMARY KEY AUTOINCREMENT,
    zip_code_id INTEGER NOT NULL,
    city_id INTEGER NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (zip_code_id) REFERENCES zip_code(zip_code_id) ON DELETE CASCADE,
    FOREIGN KEY (city_id) REFERENCES city_name(city_name_id) ON DELETE CASCADE,
    UNIQUE(zip_code_id, city_id)
);

-- Organisations-Tabellen
CREATE TABLE company (
    company_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL,
    name2 VARCHAR(50),
    street VARCHAR(50),
    city VARCHAR(50),
    zip_code INTEGER,
    phone_number VARCHAR(50),
    fax_number VARCHAR(50),
    mobile_number VARCHAR(50),
    mail_address VARCHAR(50),
    active BOOLEAN DEFAULT 1,
    employee_id INTEGER,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (employee_id) REFERENCES employee(employee_id)
);

CREATE TABLE department (
    department_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL,
    priority INTEGER DEFAULT 0,
    printer_id INTEGER,
    fax_id INTEGER,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (printer_id) REFERENCES printer(printer_id),
    FOREIGN KEY (fax_id) REFERENCES fax(fax_id)
);

-- Geräte-Basistabellen
CREATE TABLE device_type (
    device_type_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL UNIQUE,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE device_name (
    device_name_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL UNIQUE,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE manufacturer (
    manufacturer_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL,
    name2 VARCHAR(50),
    supporter VARCHAR(50),
    address VARCHAR(50),
    address2 VARCHAR(50),
    zip_city_id INTEGER,
    mail_address VARCHAR(50),
    phone_number VARCHAR(50),
    fax_number VARCHAR(50),
    hotline_number VARCHAR(50),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (zip_city_id) REFERENCES zip_city(zip_city_id)
);

-- Computer und Software
CREATE TABLE computer (
    computer_id INTEGER PRIMARY KEY AUTOINCREMENT,
    device_name_id INTEGER NOT NULL,
    serial_number VARCHAR(20) UNIQUE,
    service_tag VARCHAR(20),
    service_number VARCHAR(20),
    memory INTEGER,
    network VARCHAR(30),
    network_name VARCHAR(30),
    network_ip_address VARCHAR(30),
    active BOOLEAN DEFAULT 1,
    replace BOOLEAN DEFAULT 0,
    device_type_id INTEGER NOT NULL,
    employee_id INTEGER,
    place_id INTEGER,
    department_id INTEGER,
    manufacturer_id INTEGER,
    inventory_id INTEGER,
    processor_id INTEGER,
    os_id INTEGER,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (device_name_id) REFERENCES device_name(device_name_id),
    FOREIGN KEY (device_type_id) REFERENCES device_type(device_type_id),
    FOREIGN KEY (employee_id) REFERENCES employee(employee_id),
    FOREIGN KEY (place_id) REFERENCES place(place_id),
    FOREIGN KEY (department_id) REFERENCES department(department_id),
    FOREIGN KEY (manufacturer_id) REFERENCES manufacturer(manufacturer_id),
    FOREIGN KEY (processor_id) REFERENCES processor(processor_id),
    FOREIGN KEY (os_id) REFERENCES os(os_id)
);

--- TODO: Improve like above
---
CREATE TABLE account (
    account_id INTEGER PRIMARY KEY,
    user_name VARCHAR(50),
    password VARCHAR(64),
    system_data_id INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE chip_card (
    chip_card_id INTEGER PRIMARY KEY,
    number VARCHAR(10),
    chip_card_door_id INTEGER,
    chip_card_profile_id INTEGER,
    employee_id INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE chip_card_door (
    chip_card_door_id INTEGER PRIMARY KEY,
    number VARCHAR(10),
    place_id INTEGER,
    department_id INTEGER,
    employee_id INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE chip_card_profile (
    chip_card_profile_id INTEGER PRIMARY KEY,
    number VARCHAR(10),
    chip_card_door_id INTEGER,
    employee_id INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE chip_card_profile_door (
    chip_card_profile_door_id INTEGER PRIMARY KEY,
    chip_card_profile_id INTEGER,
    chip_card_door_id INTEGER,
    last_update TIMESTAMP
);

CREATE TABLE computer_software (
    computer_software_id INTEGER PRIMARY KEY,
    computer_id INTEGER,
    software_id INTEGER,
    last_update TIMESTAMP
);

CREATE TABLE document (
    document_id INTEGER PRIMARY KEY,
    name VARCHAR,
    document_data BLOB,
    last_update TIMESTAMP
);
CREATE TABLE employee_account (
    employee_account_id INTEGER PRIMARY KEY,
    employe_id INTEGER,
    account_id INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE employee (
    employee_id INTEGER PRIMARY KEY,
    employee_nr INTEGER,
    gender INTEGER,
    title_id INTEGER,
    first_name VARCHAR(50),
    last_name VARCHAR(50),
    birth_day DATE,
    address VARCHAR(50),
    zip_city_id INTEGER,
    home_phone VARCHAR(50),
    home_mobile VARCHAR(50),
    home_mail_address VARCHAR(50),
    business_mail_address VARCHAR(50),
    data_care BOOLEAN,
    active BOOLEAN,
    photo BLOB,
    notes VARCHAR(200),
    hire_date DATE,
    end_date DATE,
    department_id INTEGER,
    function_id INTEGER,
    computer_id INTEGER,
    printer_id INTEGER,
    phone_id INTEGER,
    mobile_id INTEGER,
    fax_id INTEGER,
    employee_account_id INTEGER,
    employee_document_id INTEGER,
    chip_card_id INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE employee_document (
    employee_document_id INTEGER PRIMARY KEY,
    employe_id INTEGER,
    document_id INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE fax (
    fax_id INTEGER PRIMARY KEY,
    device_name_id INTEGER,
    serial_number (20),
    number VARCHAR(50),
    pin VARCHAR(10),
    active BOOLEAN,
    replace BOOLEAN,
    device_type_id INTEGER,
    employee_id INTEGER,
    place_id INTEGER,
    department_id INTEGER,
    manufacturer_id INTEGER,
    inventory_id INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE function (
    function_id INTEGER PRIMARY KEY,
    name VARCHAR(50),
    priority INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE inventory (
    inventory_id INTEGER PRIMARY KEY,
    number VARCHAR(25),
    text VARCHAR(50),
    active BOOLEAN,
    last_update TIMESTAMP
);

CREATE TABLE mobile (
    mobile_id INTEGER PRIMARY KEY,
    device_name_id INTEGER,
    serial_number VARCHAR(20),
    number VARCHAR(50),
    pin VARCHAR(25),
    card_number VARCHAR(50),
    active BOOLEAN,
    replace BOOLEAN,
    device_type_id INTEGER,
    employe_id INTEGER,
    place_id INTEGER,
    department_id INTEGER,
    manufacturer_id INTEGER,
    inventory_id INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE os (
    os_id INTEGER PRIMARY KEY,
    name VARCHAR(50),
    version VARCHAR(20),
    revision VARCHAR(20),
    fix VARCHAR(20),
    last_update TIMESTAMP
);
CREATE TABLE phone (
    phone_id INTEGER PRIMARY KEY,
    device_name_id INTEGER,
    serial_number VARCHAR(25),
    number VARCHAR,
    pin VARCHAR(20),
    active BOOLEAN,
    replace BOOLEAN,
    device_type_id INTEGER,
    employe_id INTEGER,
    place_id INTEGER,
    department_id INTEGER,
    manufacturer_id INTEGER,
    inventory_id INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE place (
    place_id INTEGER PRIMARY KEY,
    name VARCHAR(50),
    room VARCHAR(50),
    desk VARCHAR(50),
    last_update TIMESTAMP
);
CREATE TABLE printer (
    printer_id INTEGER PRIMARY KEY,
    device_name_id INTEGER,
    serial_number VARCHAR(25),
    network VARCHAR(50),
    network_name VARCHAR(50),
    network_ip_address VARCHAR(50),
    active BOOLEAN,
    replace BOOLEAN,
    resources VARCHAR(50),
    paper_size_max VARCHAR(50),
    color BOOLEAN,
    device_type_id INTEGER,
    employe_id INTEGER,
    place_id INTEGER,
    department_id INTEGER,
    manufacturer_id INTEGER,
    inventory_id INTEGER,
    computer_id INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE processor (
    processor_id INTEGER PRIMARY KEY,
    name VARCHAR(50),
    clock_rate DECIMAL,
    cores INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE software (
    software_id INTEGER PRIMARY KEY,
    name VARCHAR(50),
    version VARCHAR(25),
    revision VARCHAR(25),
    fix VARCHAR(25),
    last_update TIMESTAMP
);
CREATE TABLE system_data (
    system_data_id INTEGER PRIMARY KEY,
    name VARCHAR(50),
    local BOOLEAN,
    company_id INTEGER,
    last_update TIMESTAMP
);
CREATE TABLE title (
    title_id INTEGER PRIMARY KEY,
    name VARCHAR(50),
    from_date DATE,
    last_update TIMESTAMP
);

-- Versionssteuerung
CREATE TABLE database_version (
    database_version_id INTEGER PRIMARY KEY AUTOINCREMENT,
    version VARCHAR(10) NOT NULL,
    revision VARCHAR(10) NOT NULL,
    patch VARCHAR(10) NOT NULL,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Indices für häufig verwendete Abfragen
CREATE INDEX idx_computer_active ON computer(active);
CREATE INDEX idx_computer_device_name ON computer(device_name_id);
CREATE INDEX idx_computer_employee ON computer(employee_id);

-- Triggers für automatic updated_at
CREATE TRIGGER update_computer_timestamp 
AFTER UPDATE ON computer
BEGIN
    UPDATE computer SET updated_at = CURRENT_TIMESTAMP 
    WHERE computer_id = NEW.computer_id;
END;

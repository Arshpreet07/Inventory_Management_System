# Inventory Management System

A complete Inventory Management System developed in C++, integrating MySQL to demonstrate robust back-end interaction, object-oriented programming, and modular software architecture. This project simulates real-world retail operations with role-based access, data persistence, category-based product handling, and discount management.

## Technologies & Tools
- **Programming Language:** C++
- **Core Concepts:**
  - Object-Oriented Programming (OOP)
  - Encapsulation, Polymorphism, and Dynamic Dispatch
  - SQL Queries and Joins
  - File Handling for Bulk Upload
- **IDE:** Visual Studio Code / Terminal
- - **Build System:** Manual G++ compilation via tasks.json (`Cmd+Shift+B` on Mac / `Ctrl+Shift+B` on Windows)

## Features
- **User Management**
  - Admin and employee login with role-based access.
  - Add new users (admin-only).
  - Session handling via login/logout options.
- **Inventory Management**
  - Add, update, delete, and restock items across categories.
  - Automatically restock low-stock items.
  - View all items or filter by category, stock, name, or price range.
- **Category-Specific Design**
  - Inventory supports dynamic product types:
    - Clothing: Includes fabric type and size.
    - Electronics: Includes warranty period.
    - Shoes: Includes brand and shoe size.
-**Discount System**
  - Apply flat or percentage discounts per item or entire category.
  - Discount logic integrated with pricing on display and update.
-**Bulk Operations**
  - Perform bulk inventory updates via an input file (inventory.txt).
  - Easily import multiple items using pre-defined formats.

## Project Structure
- **Classes Used:**
  - InventoryItem: Base abstract class for all product types.
  - Clothing, Electronics, Shoes: Derived classes with category-specific attributes.
  - InventoryManager: Core system for inventory control and queries.
  - UserManager: Handles authentication and user roles.
  - CategoryRegistry: Factory-like class to manage category-specific item creation.
  - DatabaseConnection: Manages MySQL connectivity.
- **Database Schema Includes:**
  - Users table (for login and role management)
  - Inventory table (master product info)
  - Clothing, Electronics, Shoes tables (category-specific fields)
  - Relational integrity via item_id foreign keys

___________________________________________________________________________________
| File/Class            | Description |
|-----------------------|-------------|
| `InventoryItem.*`     | Abstract base class for inventory items |
| `Clothing`, `Shoes`, `Electronics` | Derived classes with custom attributes |
| `InventoryManager.*`  | Handles item management, searching, discounting |
| `UserManager.*`       | Authenticates users and manages login roles |
| `DatabaseConnection.*`| Handles MySQL connection logic |
| `main.cpp`            | Entry point and full user interaction menu |
___________________________________________________________________________________


## Core Functionalities
- Full MySQL integration using Connector/C++.
- Role-based logic for user vs admin permissions.
- File-based bulk upload support.
- SQL query optimization for search and filtering.

  
## Sample Menu Options

<img width="1097" alt="Screenshot 2025-06-07 at 12 47 30 PM" src="https://github.com/user-attachments/assets/77aba819-ffe1-4ce7-837a-807df320bc95" />


## Database Schema (SQL Setup)

Before running the project, set up the required MySQL tables using the following SQL script:

<details>
<summary>Click to view full schema</summary>

```sql
CREATE TABLE Users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(255) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    role VARCHAR(50) NOT NULL
);

CREATE TABLE Inventory (
    item_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    stock_quantity INT NOT NULL,
    category VARCHAR(50) NOT NULL,
    restock_threshold INT DEFAULT 5,
    restock_amount INT DEFAULT 10,
    discount_type varchar(20) DEFAULT 'None',
    discount_value DECIMAL(10,2) DEFAULT 0.0
);

CREATE TABLE Clothing (
    item_id INT PRIMARY KEY,
    fabric_type VARCHAR(50),
    size VARCHAR(10),
    FOREIGN KEY (item_id) REFERENCES Inventory(item_id) ON DELETE CASCADE
);

create table Electronics(
    electronics_id INT PRIMARY KEY,
    warranty_period INT NOT NULL,
    FOREIGN KEY (electronics_id) REFERENCES Inventory(item_id) ON DELETE CASCADE
);

create table Shoes(
    shoes_id INT PRIMARY KEY,
    brand VARCHAR(100) NOT NULL,
    size VARCHAR(20) NOT NULL,
    FOREIGN KEY (shoes_id) REFERENCES Inventory(item_id) ON DELETE CASCADE
);

CREATE TABLE AuditLogs (
    id INT AUTO_INCREMENT PRIMARY KEY,
    event TEXT NOT NULL,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE Transactions (
     transaction_id INT PRIMARY KEY AUTO_INCREMENT,
     item_id INT NOT NULL,
     quantity INT NOT NULL,
     price DECIMAL(10,2) NOT NULL,
     type VARCHAR(10) NOT NULL,
     timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
);
```
</details>

You may also save the above as `schema.sql` and run it in MySQL CLI or Workbench:

```bash
mysql -u root -p < schema.sql
```

---

## Build & Run Instructions
### Compile

```bash
g++ -std=c++17 -o invSys main.cpp Clothing.cpp Electronics.cpp InventoryItem.cpp InventoryManager.cpp UserManager.cpp User.cpp TransactionManager.cpp Shoes.cpp DatabaseConnection.cpp -I/usr/local/mysql-connector-c++-9.2.0/include/ -L/usr/local/mysql-connector-c++-9.2.0/lib64 -lmysqlcppconn
```

If using `tasks.json`, just press:

- `Cmd+Shift+B` on macOS  
- `Ctrl+Shift+B` on Windows

### Run (macOS)

Set MySQL connector runtime path **before running**:

```bash
export DYLD_LIBRARY_PATH=/usr/local/mysql-connector-c++-9.2.0/lib64:$DYLD_LIBRARY_PATH
./invSys
```

### Run (Windows)

Ensure `mysqlcppconn.dll` is in the same folder or available in system `PATH`, then run the executable:

```bash
invSys.exe
```

---


## Motivation Behind the Project
This Inventory Management System was developed to simulate real-world retail inventory workflows with a focus on modern backend integration. Instead of relying on flat file storage or static logic, this project connects directly to a **relational MySQL database**, enabling persistent storage, multi-category product support, and SQL-powered queries. The aim was to build a complete command-line based application that reflects how real systems manage inventory — scalable, modular, and extensible for real business use cases, while staying within the console environment for simplicity and control.

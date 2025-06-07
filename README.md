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



## Database Schema (SQL Setup)

Before running the project, set up the required MySQL tables using the following SQL script:

<details>
<summary>Click to view full schema</summary>

```sql
CREATE TABLE Users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(255) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    role VARCHAR(50) NOT NULL
);

CREATE TABLE Inventory (
    id INT PRIMARY KEY,
    name VARCHAR(255),
    price DOUBLE,
    stock INT,
    minStock INT,
    discountType VARCHAR(50),
    discountValue DOUBLE,
    category VARCHAR(50)
);

CREATE TABLE Clothing (
    itemId INT PRIMARY KEY,
    fabricType VARCHAR(100),
    size VARCHAR(10),
    FOREIGN KEY (itemId) REFERENCES Inventory(id) ON DELETE CASCADE
);

CREATE TABLE Electronics (
    itemId INT PRIMARY KEY,
    warrantyPeriod INT,
    FOREIGN KEY (itemId) REFERENCES Inventory(id) ON DELETE CASCADE
);

CREATE TABLE Shoes (
    itemId INT PRIMARY KEY,
    shoeType VARCHAR(100),
    size VARCHAR(10),
    FOREIGN KEY (itemId) REFERENCES Inventory(id) ON DELETE CASCADE
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

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Producto {
protected:
    string nombre;
    string marca;
    int precio;
    int descuento;
    int cantidadInventario;

public:
    Producto(string _nombre, string _marca, int _precio, int _descuento, int _cantidad)
        : nombre(_nombre), marca(_marca), precio(_precio), descuento(_descuento), cantidadInventario(_cantidad) {}

    virtual void mostrarInformacion() const = 0;
    virtual ~Producto() {}

    void modificarProducto() {
        cout << "Modificar nombre (" << nombre << "): ";
        getline(cin, nombre);
        cout << "Modificar precio (" << precio << "): ";
        cin >> precio;
        cin.ignore();
        cout << "Modificar descuento (" << descuento << "%): ";
        cin >> descuento;
        cin.ignore();
    }

    int getPrecio() const {
        return precio;
    }

    int getCantidadInventario() const {
        return cantidadInventario;
    }

    void venderProducto(int cantidad) {
        if (cantidad <= cantidadInventario) {
            cantidadInventario -= cantidad;
        } else {
            cout << "No hay suficiente inventario." << endl;
        }
    }

    string getNombre() const {
        return nombre;
    }

    int getDescuento() const {
        return descuento;
    }
};

class Comestible : public Producto {
private:
    string fechaVencimiento;

public:
    Comestible(string _nombre, string _marca, int _precio, int _descuento, int _cantidad, string _fecha)
        : Producto(_nombre, _marca, _precio, _descuento, _cantidad), fechaVencimiento(_fecha) {}

    void mostrarInformacion() const override {
        cout << "Comestible - Nombre: " << nombre << ", Marca: " << marca
             << ", Precio: $" << precio << ", Descuento: " << descuento
             << "%, Inventario: " << cantidadInventario << ", Fecha de vencimiento: " << fechaVencimiento << endl;
    }
};

class Electronico : public Producto {
private:
    int garantiaMeses;

public:
    Electronico(string _nombre, string _marca, int _precio, int _descuento, int _cantidad, int _garantia)
        : Producto(_nombre, _marca, _precio, _descuento, _cantidad), garantiaMeses(_garantia) {}

    void mostrarInformacion() const override {
        cout << "Electronico - Nombre: " << nombre << ", Marca: " << marca
             << ", Precio: $" << precio << ", Descuento: " << descuento
             << "%, Inventario: " << cantidadInventario << ", Garantia: " << garantiaMeses << " meses" << endl;
    }
};

class Aseo : public Producto {
private:
    string material;

public:
    Aseo(string _nombre, string _marca, int _precio, int _descuento, int _cantidad, string _material)
        : Producto(_nombre, _marca, _precio, _descuento, _cantidad), material(_material) {}

    void mostrarInformacion() const override {
        cout << "Aseo - Nombre: " << nombre << ", Marca: " << marca
             << ", Precio: $" << precio << ", Descuento: " << descuento
             << "%, Inventario: " << cantidadInventario << ", Material: " << material << endl;
    }
};

void mostrarProductos(const vector<Producto*>& productos) {
    for (const Producto* producto : productos) {
        producto->mostrarInformacion();
    }
}

vector<Producto*> cargarDatos() {
    vector<Producto*> productos;
    int opcion;
    string nombre, marca, fechaVencimiento, material;
    int precio, descuento, cantidad, garantia;
    do {
        cout << "Seleccione el tipo de producto a agregar (1-Comestible, 2-Electronico, 3-Aseo, 0-Terminar): ";
        cin >> opcion;
        cin.ignore();

        if (opcion == 0) break;

        cout << "Ingrese el nombre del producto: ";
        getline(cin, nombre);
        cout << "Ingrese la marca: ";
        getline(cin, marca);
        cout << "Ingrese el precio: ";
        cin >> precio;
        cout << "Ingrese el descuento (%): ";
        cin >> descuento;
        cout << "Ingrese la cantidad en inventario: ";
        cin >> cantidad;
        cin.ignore();

        switch (opcion) {
            case 1:
                cout << "Ingrese la fecha de vencimiento (MM/AAAA): ";
                getline(cin, fechaVencimiento);
                productos.push_back(new Comestible(nombre, marca, precio, descuento, cantidad, fechaVencimiento));
                break;
            case 2:
                cout << "Ingrese la garantia (meses): ";
                cin >> garantia;
                cin.ignore();
                productos.push_back(new Electronico(nombre, marca, precio, descuento, cantidad, garantia));
                break;
            case 3:
                cout << "Ingrese el material: ";
                getline(cin, material);
                productos.push_back(new Aseo(nombre, marca, precio, descuento, cantidad, material));
                break;
        }
    } while (opcion != 0);

    return productos;
}

void modificarProductos(vector<Producto*>& productos) {
    if (productos.empty()) {
        cout << "No hay productos para modificar." << endl;
        return;
    }
    mostrarProductos(productos);
    cout << "Ingrese el nombre del producto a modificar: ";
    string nombre;
    getline(cin, nombre);
    for (auto& producto : productos) {
        if (producto->getNombre() == nombre) {
            producto->modificarProducto();
            return;
        }
    }
    cout << "Producto no encontrado." << endl;
}

void realizarCompra(vector<Producto*>& productos) {
    if (productos.empty()) {
        cout << "No hay productos disponibles para la compra." << endl;
        return;
    }

    vector<pair<Producto*, int>> carrito;
    while (true) {
        cout << "Productos disponibles para compra:" << endl;
        for (const Producto* producto : productos) {
            cout << producto->getNombre() << " - ";
            if (producto->getCantidadInventario() > 0) {
                cout << "Disponible" << endl;
            } else {
                cout << "Agotado" << endl;
            }
        }

        cout << "Ingrese el nombre del producto a comprar o 'finalizar' para terminar: ";
        string nombre;
        getline(cin, nombre);
        if (nombre == "finalizar") {
            break;
        }

        auto producto_encontrado = find_if(productos.begin(), productos.end(), [&nombre](const Producto* p) {
            return p->getNombre() == nombre;
        });

        if (producto_encontrado == productos.end()) {
            cout << "Producto no encontrado." << endl;
            continue;
        }

        if ((*producto_encontrado)->getCantidadInventario() == 0) {
            cout << "El producto seleccionado está agotado y no puede ser añadido al carrito." << endl;
            continue;
        }

        cout << "Ingrese la cantidad: ";
        int cantidad;
        cin >> cantidad;
        cin.ignore();

        if (cantidad > (*producto_encontrado)->getCantidadInventario()) {
            cout << "Cantidad solicitada no disponible. Solo hay " << (*producto_encontrado)->getCantidadInventario() << " unidades en inventario." << endl;
        } else {
            (*producto_encontrado)->venderProducto(cantidad);
            carrito.push_back(make_pair(*producto_encontrado, cantidad));
            cout << "Producto aniadido al carrito." << endl;
        }
    }

    cout << "Recibo de compra:" << endl;
    int total = 0;
    for (auto& item : carrito) {
        int subtotal = item.first->getPrecio() * item.second * (100 - item.first->getDescuento()) / 100;
        cout << item.first->getNombre() << " x " << item.second << " = $" << subtotal << endl;
        total += subtotal;
    }
    cout << "Total: $" << total << endl;
}

int main() {
    vector<Producto*> productos;
    int opcion;
    do {
        cout << "Menu Principal:\n1. Agregar Productos\n2. Modificar Productos\n3. Realizar Compra\n0. Salir\nIngrese una opcion: ";
        cin >> opcion;
        cin.ignore();
        switch (opcion) {
            case 1:
                productos = cargarDatos();
                break;
            case 2:
                modificarProductos(productos);
                break;
            case 3:
                realizarCompra(productos);
                break;
            case 0:
                break;
            default:
                cout << "Opcion invalida. Por favor, seleccione nuevamente." << endl;
        }
    } while (opcion != 0);

    for (Producto* producto : productos) {
        delete producto;
    }
    productos.clear();

    return 0;
}

// Created by jordy on 31/03/2021.

#include "Inventario.h"

//      CONSTRUCTOR + DESTRUCTOR
Inventario::Inventario()
{
    identificador = 0;
    nombre = "Indefinido";
    listaProducto = new ListaProducto();
    caja = new Caja();
}

[[maybe_unused]] Inventario::Inventario(int xIdentificador, const string &xNombre, ListaProducto &xListaProducto, Caja &xCaja)
{
    identificador = xIdentificador;
    nombre = xNombre;
    listaProducto = &xListaProducto;
    caja = &xCaja;
}

Inventario::~Inventario()
{
    delete listaProducto;
    delete caja;
}


//      SETTERS
[[maybe_unused]] void Inventario::setIdentificador(int xIdentificador)
{
    identificador = xIdentificador;
}

[[maybe_unused]] void Inventario::setNombre(const string &xNombre)
{
    nombre = xNombre;
}

[[maybe_unused]] void Inventario::setListaProducto(ListaProducto &xListaProducto)
{
    listaProducto = &xListaProducto;
}

[[maybe_unused]] void Inventario::setCaja(Caja &xCaja)
{
    caja = &xCaja;
}


//      GETTERS + TOSTRING
[[maybe_unused]] [[nodiscard]] int Inventario::getIdentificador() const
{
    return identificador;
}

[[maybe_unused]] [[nodiscard]] string Inventario::getNombre() const
{
    return nombre;
}

[[maybe_unused]] [[nodiscard]] ListaProducto *Inventario::getListaProducto() const
{
    return listaProducto;
}

[[maybe_unused]] [[nodiscard]] Caja *Inventario::getCaja() const
{
    return caja;
}

[[maybe_unused]] [[nodiscard]] string Inventario::toString() const
{
    stringstream s;

    s << endl << "   - Inventario -" << endl;
    s << "-< Identificador: " << identificador << endl;
    s << "-< Nombre: " << nombre << endl;
    s << listaProducto->toStringListaProducto() << endl;
    //s<<caja->toString(); Por seguridad de el/la cajero/a, no debería ser mostrado. consultarDinero() es accesible desde menuAdministrador

    return s.str();
}


//  HERENCIAS DE IADMINISTRADOR
void Inventario::insertar(Producto &xProducto)
{
    listaProducto->agregar(xProducto);
}

bool Inventario::agregarCantidades(const string &xNombre, int xCantidad)
{
    //"Si el producto no está en la lista"
    if(listaProducto->obtenerProductoPorNombre(xNombre) == nullptr)
    { return false; }
    else
    {
        int cantidadAnterior = listaProducto->obtenerProductoPorNombre(xNombre)->getCantidad();
        listaProducto->obtenerProductoPorNombre(xNombre)->setCantidad(cantidadAnterior + xCantidad);
    }
    return true;
}

bool Inventario::disminuirCantidades(const string &xNombre, int xCantidad)
{
    //"Si el producto no está en la lista"
    if(listaProducto->obtenerProductoPorNombre(xNombre) == nullptr)
    { return false; }
    else
    {
        int cantidadAnterior = listaProducto->obtenerProductoPorNombre(xNombre)->getCantidad();
        listaProducto->obtenerProductoPorNombre(xNombre)->setCantidad(cantidadAnterior - xCantidad);
        if(cantidadAnterior - xCantidad < 0)//Si cantidad da negativa
        { listaProducto->obtenerProductoPorNombre(xNombre)->setCantidad(0); }
    }
    return true;
}

void Inventario::borrar(const string &xNombre)
{
    listaProducto->eliminar(xNombre);
}

Producto *Inventario::consultar(const string &xNombre)
{
    return listaProducto->obtenerProductoPorNombre(xNombre);
}

float Inventario::consultarDinero() const
{
    return caja->getDinero();
}

void Inventario::ingresarDinero(float xDinero)
{
    caja->setDinero(caja->getDinero() + xDinero);
}

void Inventario::retirarDinero(float xDinero)
{
    if(caja->getDinero() - xDinero >= 0)
    { caja->setDinero(caja->getDinero() - xDinero); }
    else
    { caja->setDinero(0); }
}


//  HERENCIAS DE IVENDEDOR
string Inventario::realizarCompra(int xPosicion, int xCantidad, float xPago)
{
    if(getListaProducto()->listaVacia()){ return "(!) No hay productos registrados para comprar...\n"; }
    if(getListaProducto()->obtenerProductoPorPosicion(xPosicion)==nullptr) { return "(!) No exite un producto en la posicion digitada...\n"; }
    Producto &producto = *getListaProducto()->obtenerProductoPorPosicion(xPosicion);
    if (producto.getCantidad() < xCantidad) { return "(!) No hay suficiente cantidad de este producto...\n"; }
    if (producto.getPrecio()*xCantidad > xPago) { return "(!) Pago insuficiente...\n"; }

    //Realiza compra
    disminuirCantidades(producto.getNombre(),xCantidad);
    ingresarDinero(producto.getPrecio()*xCantidad);
    return caja->desgloceVuelto(xPago,producto.getPrecio()*xCantidad);
}

Producto *Inventario::consultar(int xPosicion)
{
    return listaProducto->obtenerProductoPorPosicion(xPosicion);
}
class ICalendarizador
{
    public:
        virtual ~ICalendarizador() {}
        virtual Proceso Siguiente() = 0;
};

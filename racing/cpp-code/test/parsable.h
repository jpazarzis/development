// parsable.h
#ifndef PARSABLE_INCLUDED
#define PARSABLE_INCLUDED
#include <fstream>
#include <sstream>
class parsable
{
    public:

        inline std::string get_field_as_str(const int index) const
        {
            return &_buffer [_index[index]];
        }

        template <class T>
        inline T get_field_as_num ( const int index ) const
        {                              
            std::stringstream ss(&_buffer [_index[index]]);
            T result;
            return ss >> result ? result : 0;
        }

        inline size_t fields_count() const
        {
            return _index.size();
        }

        inline size_t length() const
        {
            return _buffer.length();
        }

    private:

        inline void parseit()
        {
            const int length = _buffer.length();

            bool inside_string = false; 
            int  cur_field_starting_index = 0;

            for(register int i = 0; i < length; ++i)
            {
                switch (_buffer[i])
                {
                    case '"' :
                        if(!inside_string)
                            cur_field_starting_index = i +1;
                        else
                            _buffer[i] = ' ';
                        inside_string = !inside_string;
                        break;
                    case ',':
                        if(!inside_string)
                        {   
                            _index.push_back(eat_forward_spaces(cur_field_starting_index, length));
                            cur_field_starting_index = i + 1;
                            _buffer[i] = ' ';
                            _buffer[eat_backward_spaces(i)] = '\0';
                        }
                        break;
                    default:
                        break; 
                }           
            }
        }

        inline int eat_forward_spaces(int i, const int length) const
        {
            for(; i < length && _buffer[i] == ' '; ++i);
            return i;
        }
        

        inline int eat_backward_spaces(int i) const
        {
            for(; i >=0 && _buffer[i] == ' '; --i);
            return i+1;
        }

        inline void populate(std::ifstream& stm)
        { 
            _index.clear();
            getline(stm, _buffer);
            parseit();
        }

        parsable() {}

        std::string _buffer;
        std::vector<int> _index;
        parsable(const parsable& other);        
        parsable& operator=(const parsable& other);

        friend class horse;
};

#endif // PARSABLE_INCLUDED

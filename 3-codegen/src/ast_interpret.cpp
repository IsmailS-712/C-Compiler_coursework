#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match(program->type, reNum) ){
        return std::atol(program->type.c_str());
    }
        
    // TODO : Check for things matching reId
    else if( regex_match(program->type, reId) ){
        return context.bindings[program->type.c_str()];
    }
        
    else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;
    }
    else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;
    }

    // TODO: Handle other constructs
        
    else if (program->type=="Input"){
        int32_t val_input;
        std::cin>>val_input;
        return val_input;
    }
    else if (program->type=="Add"){
        int32_t val_add1 = Interpret(context, program->branches.at(0));
        int32_t val_add2 = Interpret(context, program->branches.at(1));
        return val_add1 + val_add2;
    }
    else if (program->type=="Sub"){
        int32_t val_sub1 = Interpret(context, program->branches.at(0));
        int32_t val_sub2 = Interpret(context, program->branches.at(1));
        return val_sub1 - val_sub2;  
    }
    else if (program->type=="LessThan"){
        int32_t val_lessthan1 = Interpret(context, program->branches.at(0));
        int32_t val_lessthan2 = Interpret(context, program->branches.at(1));
        return val_lessthan1 < val_lessthan2;  
    }
    else if (program->type=="If"){ 
        int32_t val_if = Interpret(context, program->branches.at(0));
        if(val_if ==0){
            int32_t val_if2 = Interpret(context, program->branches.at(2));
            return val_if2;
        }
        else{
            int32_t val_if1 = Interpret(context, program->branches.at(1));
            return val_if1;
        }
    }
    else if (program->type=="While"){
        int32_t C;
        while(Interpret(context, program->branches.at(0))!=0){
            C = Interpret(context, program->branches.at(1));

        }
        return 0;
    }
    else if (program->type=="Assign"){
        int32_t assign = Interpret(context, program->branches.at(0));
        if(context.bindings.find(program->value)!=context.bindings.end()){
            context.bindings[program->value]=assign;
        }
        else{
            context.bindings.insert({program->value,assign});
        }
        return assign;
    }
    else if (program->type=="Seq"){
        int32_t itop;
        int32_t count = program->branches.size();
        for(unsigned i = 0; i<count; i++){
            itop = Interpret(context, program->branches.at(i));
        }
        return itop;
    }
    else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}

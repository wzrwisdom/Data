import os
import re
import yaml

# === 配置路径 ===
CONFIG_FILE = "../future_factors_config.yml"
OUTPUT_DIR = "generated_future_factors"

# === 模板字符串 ===
HEADER_TEMPLATE = '''#ifndef {macro_name}
#define {macro_name}

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class {class_name} : public BaseFactor {{
public:
    {class_name}() : BaseFactor("{factor_key}") {{}}

    void setParams(const std::unordered_map<std::string, double>& params) override {{
        {param_block}
    }}

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {{
        // TODO: implement logic for {factor_key}
        result_ = 0.0;
    }}

private:
    {param_decls}
}};

REGISTER_FACTOR({class_name});

#endif // {macro_name}
'''

# === 名称转换函数 ===
def to_macro(name):
    return re.sub(r'[^a-zA-Z0-9]', '_', name).upper() + "_H"

def to_class_name(name):
    return ''.join(x.capitalize() for x in name.split('_')) + "Factor"

def to_filename(name):
    return name + "_factor.hpp"

# === 主函数 ===
def generate_headers(config_file: str, output_dir: str):
    with open(config_file, 'r', encoding='utf-8') as f:
        config_data = yaml.safe_load(f)

    os.makedirs(output_dir, exist_ok=True)

    for factor_key, params in config_data.items():
        class_name = to_class_name(factor_key)
        macro_name = to_macro(factor_key)
        filename = to_filename(factor_key)
        filepath = os.path.join(output_dir, filename)

        if isinstance(params, dict):
            param_block = ""
            param_decls = ""
            for pname in params:
                param_block += f'        if (params.count("{pname}")) {pname} = static_cast<int>(params.at("{pname}"));\n'
                param_decls += f'    int {pname} = 1;\n'
        else:
            param_block = "        // No parameters"
            param_decls = ""

        header_code = HEADER_TEMPLATE.format(
            macro_name=macro_name,
            class_name=class_name,
            factor_key=factor_key,
            param_block=param_block.strip(),
            param_decls=param_decls.strip()
        )

        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(header_code)

    print(f"✅ Generated {len(config_data)} factor headers in '{output_dir}'.")

# === 执行 ===
if __name__ == "__main__":
    generate_headers(CONFIG_FILE, OUTPUT_DIR)

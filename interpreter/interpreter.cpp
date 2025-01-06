// interpreter.cpp: 定义应用程序的入口点。
//

#include "interpreter.h"

using namespace std;

// token for lexical analysis
//
struct Token {
	enum Type { integer, plus, minus, lparen, rparen } type;
	string text;
	explicit Token(Type type, const string& text) :type{ type }, text{ text } {}

	friend ostream& operator<<(ostream& os, const Token& obj)
	{
		return os << "`" << obj.text << "`";
	}
};

vector<Token> lex(const string& input)
{
	vector<Token> result;
	for (int i = 0; i < input.size(); ++i) {
		switch (input[i])
		{
		case '+':
			result.emplace_back(Token::plus, "+");
			break;
		case '-':
			result.emplace_back(Token::minus, "-");
			break;
		case '(':
			result.emplace_back(Token::lparen, "(");
			break;
		case ')':
			result.emplace_back(Token::rparen, ")");
			break;
		default: {
			ostringstream buffer;
			buffer << input[i];
			for (int j = i+1; j < input.size(); ++j)
			{
				if (isdigit(input[j]))
				{
					buffer << input[j];
					++i;
				}
				else
				{
					result.emplace_back(Token::integer, buffer.str());
					buffer.str("");
					break;
				}
			}
			if(auto str=buffer.str();str.length() > 0)
				result.emplace_back(Token::integer, str);
		}
		}
	}
	return result;
}

struct Element
{
	virtual int eval() const = 0;
};
struct Integer : Element
{
	int value;
	explicit Integer(const int value) : value(value) {}

	int eval() const override { return value; }

};

struct BinaryOperation : Element
{
	enum Type { addition, subtraction } type;
	shared_ptr<Element> lhs, rhs;

	int eval() const override
	{
		if (type == addition)
			return lhs->eval() + rhs->eval();
		return lhs->eval() - rhs->eval();
	}
};

shared_ptr<Element> parse(const vector<Token>& tokens)
{
	auto result = make_unique<BinaryOperation>();
	bool have_lhs = false;
	for (int i = 0; i < tokens.size(); i++)
	{
		auto token = tokens[i];
		switch (token.type)
		{
			//process each of tokens
		case Token::integer: {
			int value = boost::lexical_cast<int>(token.text);
			auto integer = make_shared<Integer>(value);
			if (!have_lhs) {
				result->lhs = integer;
				have_lhs = true;
			}
			else result->rhs = integer;
			break;
		}
		case Token::plus:
			result->type = BinaryOperation::addition;
			break;
		case Token::minus:
			result->type = BinaryOperation::subtraction;
			break;
		case Token::lparen: {
			int j = i;
			for (; j < tokens.size(); ++j)
			{
				if (tokens[j].type == Token::rparen)
					break;
			}
			vector<Token> subexpression(&tokens[i + 1], &tokens[j]);
			auto element = parse(subexpression);
			if (!have_lhs)
			{
				result->lhs = element;
				have_lhs = true;
			}
			else result->rhs = element;
			i = j;//advance
			break;
		}
		default:
			break;
		}
	}
	return result;
}

void test01()
{
	string input{ "(13-4)-(12+1)" };
	auto tokens = lex(input);
	auto parsed = parse(tokens);
	cout << input << " = " << parsed->eval() << endl;
}

int main()
{
	cout << "Hello CMake." << endl;
	test01();
	return 0;
}

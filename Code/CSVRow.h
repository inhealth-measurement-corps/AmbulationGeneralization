using namespace std;

class CSVRow
{
public:
	string const& operator[](size_t index) const
	{
		return m_data[index];
	}
	size_t size() const
	{
		return m_data.size();
	}
	void readNext(istream& stream)
	{
		string line;
		string cell;
		
		getline(stream, line);
		stringstream lineStream(line);

		m_data.clear();

		while (getline(lineStream, cell, ','))
		{
			m_data.push_back(cell);
		}
		if (!lineStream && cell.empty())
		{
			m_data.push_back("");
		}
	}
private:
	vector<string> m_data;
};

istream& operator >> (istream& stream, CSVRow& data)
{
	data.readNext(stream);
	return stream;
}